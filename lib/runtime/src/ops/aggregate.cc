/* Copyright 2023 CMU, Facebook, LANL, MIT, NVIDIA, and Stanford (alphabetical)
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "aggregate.h"
#include "kernels/aggregate_kernels.h"
#include "tasks.h"
#include "kernels/profiling.h"

namespace FlexFlow {

enum Slots {
  GATE_PREDS,
  GATE_ASSIGN,
  TRUE_GATE_ASSIGN,
  FULL_GATE_GRADIENTS,
  EXP_PREDS,
  OUTPUT,
  ATTRS,
  PROFILING,
};

// declare Legion names
using Legion::ArgumentMap;
using Legion::Context;
using Legion::coord_t;
using Legion::Domain;
using Legion::FutureMap;
using Legion::IndexLauncher;
using Legion::PhysicalRegion;
using Legion::Predicate;
using Legion::Rect;
using Legion::RegionRequirement;
using Legion::Runtime;
using Legion::Task;
using Legion::TaskArgument;
using Legion::TaskLauncher;

using namespace FlexFlow::Kernels::Aggregate;

Tensor FFModel::aggregate(
    Tensor const *inputs, /* gate_preds, gate_assign, gate assign TopK,
                             full_gate_pred, exp_pred_1, ... , exp_pred_n */
    int n,
    float lambda_bal,
    char const *name) {
  Layer *li = new Layer(this,
                        OP_AGGREGATE,
                        DT_FLOAT,
                        name,
                        n + 4 /*inputs*/,
                        0 /*weights*/,
                        1 /*outputs*/,
                        inputs);
  {
    int num_dim = inputs[4]->num_dims;
    // Set output shape
    int dims[MAX_TENSOR_DIM];
    for (int i = 0; i < num_dim - 1; i++) {
      dims[i] = inputs[4]->dims[i];
    }
    dims[num_dim - 1] = inputs[0]->dims[num_dim - 1];
    li->outputs[0] = create_tensor_legion_ordering(
        num_dim, dims, DT_FLOAT, li, 0, true /*create_grad*/);
  }
  li->add_int_property("n", n);
  li->add_float_property("lambda_bal", lambda_bal);
  layers.push_back(li);
  return li->outputs[0];
}



Op *Aggregate::create_operator_from_layer(
    FFModel &model,
    Layer const *layer,
    std::vector<ParallelTensor> const &inputs) {
  long long value1;
  layer->get_int_property("n", value1);
  int n = value1;
  float value2;
  layer->get_float_property("lambda_bal", value2);
  float lambda_bal = value2;
  return new Aggregate(model, inputs.data(), n, lambda_bal, layer->name);
}

Aggregate::Aggregate(FFModel &model,
                     ParallelTensor const *_inputs,
                     int _n,
                     float _lambda_bal,
                     char const *name)
    : Op(model,
         OP_AGGREGATE,
         DT_FLOAT,
         name,
         _n + 4 /*inputs*/,
         0 /*weights*/,
         1 /*outputs*/,
         _inputs),
      attrs(_n, _lambda_bal)
{
  // FIXME: For now, set upper limits Better: Do as follows, but memory is
  // assigned per block, so requires to check that
  // https://stackoverflow.com/questions/5531247/allocating-shared-memory/5531640#5531640
  assert(attrs.n <= AGGREGATE_MAX_N && "Increase AGGREGATE_MAX_N in #define");
  assert(inputs[0]->dims[0].size <= AGGREGATE_MAX_K &&
         "Increase AGGREGATE_MAX_K in #define");
  assert(inputs[0]->dims[1].size <= AGGREGATE_MAX_BATCH_SIZE &&
         "Increase AGGREGATE_MAX_BATCH_SIZE in #define");

  assert(attrs.n + 4 == numInputs);
  assert(attrs.n > 0);
  assert(inputs[0]->num_dims == 2 + 1);
  assert(inputs[1]->num_dims == 2 + 1);
  assert(inputs[2]->num_dims == 2 + 1);
  assert(inputs[3]->num_dims == 2 + 1);

  for (int i = 0; i < inputs[0]->num_dims; i++) {
    assert(inputs[0]->dims[i] == inputs[1]->dims[i]);
    assert(inputs[0]->dims[i] == inputs[2]->dims[i]);
  }
  assert(inputs[0]->dims[1] == inputs[3]->dims[1]);
  assert(inputs[3]->dims[0].size == attrs.n);

  // expert inputs
  int num_dim = inputs[4]->num_dims;
  int out_dim = inputs[4]->dims[0].size;
  for (int i = 1; i < attrs.n; i++) {
    assert(inputs[i + 4]->num_dims == num_dim);
    assert(inputs[i + 4]->dims[0].size == out_dim);
  }
  // Set output shape
  ParallelDim dims[MAX_TENSOR_DIM];
  for (int i = 0; i < num_dim - 1; i++) {
    dims[i] = inputs[4]->dims[i];
  }
  dims[num_dim - 2] = inputs[0]->dims[num_dim - 2];
  dims[num_dim - 1] = inputs[0]->dims[num_dim - 1];
  numOutputs = 1;
  outputs[0] = model.create_parallel_tensor_legion_ordering(
      num_dim, dims, DT_FLOAT, this);

  numWeights = 0;
}

Aggregate::Aggregate(FFModel &model,
                     Aggregate const &other,
                     std::vector<ParallelTensor> const &inputs)
    : Aggregate(model, inputs.data(), other.attrs, other.name) {}

static OpTaskSignature get_init_task_signature() {
  OpTaskSignature init(OpTaskType::INIT);

  init.add_arg_slot<AggregateAttrs>(ATTRS);
  init.add_arg_slot<bool>(PROFILING);

  return init;
}

static OpTaskSignature get_fwd_task_signature() {
  OpTaskSignature fwd(OpTaskType::FWD);

  fwd.add_input_slot(GATE_PREDS, READ_WRITE);
  fwd.add_input_slot(GATE_ASSIGN, READ_WRITE);
  fwd.add_input_slot(EXP_PREDS, SlotType::VARIADIC);
  fwd.add_output_slot(OUTPUT);

  return fwd;
}

static OpTaskSignature get_bwd_task_signature() {
  OpTaskSignature bwd(OpTaskType::BWD);

  bwd.add_input_slot(GATE_PREDS, READ_WRITE);
  bwd.add_input_slot(GATE_ASSIGN);
  bwd.add_input_slot(TRUE_GATE_ASSIGN);
  bwd.add_input_grad_slot(FULL_GATE_GRADIENTS);
  bwd.add_input_slot(EXP_PREDS, SlotType::VARIADIC, READ_WRITE);
  bwd.add_input_grad_slot(EXP_PREDS, SlotType::VARIADIC);
  bwd.add_output_grad_slot(OUTPUT);

  bwd.add_arg_slot<AggregateAttrs>(ATTRS);

  return bwd;
}

TaskID Aggregate::get_init_task_id() const {
  return AGGREGATE_INIT_TASK_ID;
}

TaskID Aggregate::get_fwd_task_id() const {
  return AGGREGATE_FWD_TASK_ID;
}

TaskID Aggregate::get_bwd_task_id() const {
  return AGGREGATE_BWD_TASK_ID;
}

OpTaskBinding Aggregate::get_init_task_binding() const {
  OpTaskBinding binding;

  binding.bind_arg(ATTRS, attrs);

  return binding;
}

OpTaskBinding Aggregate::get_fwd_task_binding() const {
  OpTaskBinding binding;

  binding.bind(GATE_PREDS, input_tensor(0));
  binding.bind(GATE_ASSIGN, input_tensor(1));
  
  for (int i = 0; i < this->attrs.n; i++) {
    binding.bind(EXP_PREDS, input_tensor(i+4));
  }

  binding.bind(OUTPUT, output_tensor(0));

  return binding;
}

OpTaskBinding Aggregate::get_bwd_task_binding() const {
  OpTaskBinding binding;

  binding.bind(GATE_PREDS, input_tensor(0));
  binding.bind(GATE_ASSIGN, input_tensor(1));
  binding.bind(TRUE_GATE_ASSIGN, input_tensor(2));
  binding.bind_grad(FULL_GATE_GRADIENTS, input_tensor(3).grad());
  
  for (int i = 0; i < this->attrs.n; i++) {
    binding.bind(EXP_PREDS, input_tensor(i+4));
    binding.bind_grad(EXP_PREDS, input_tensor(i+4).grad());
  }

  binding.bind_grad(OUTPUT, output_tensor(0).grad());

  return binding;
}

void Aggregate::init(FFModel const &ff) {
  this->execute_task(ff, AGG_SPEC_INIT_TASK_ID, get_init_task_signature());
}

PerDeviceOpState *Aggregate::init_task(Task const *task,
                             std::vector<PhysicalRegion> const &regions,
                             Context ctx,
                             Runtime *runtime) {
  OpTaskArgumentAccessor acc(task, regions, ctx, runtime);

  AggregateAttrs const &attrs = acc.get_argument<AggregateAttrs>(ATTRS);
  bool profiling = acc.get_argument<bool>(PROFILING);

  FFHandler handle = *((FFHandler *)task->local_args);
  AggregatePerDeviceState *m = new AggregatePerDeviceState(handle, attrs.n);

  m->profiling = profiling;
  return m;
}


void Aggregate::forward(FFModel const &ff) {
  this->execute_task(ff, AGGREGATE_FWD_TASK_ID, get_fwd_task_signature());
}

void Aggregate::forward_task(Task const *task,
                             std::vector<PhysicalRegion> const &regions,
                             Context ctx,
                             Runtime *runtime) {
  OpTaskArgumentAccessor acc(task, regions, ctx, runtime);

  AggregateAttrs const &attrs = acc.get_argument<AggregateAttrs>(ATTRS);
  int n = attrs.n;

  assert((int)regions.size() == n + 3);
  assert((int)task->regions.size() == n + 3);

  AggregatePerDeviceState const *m = *((AggregatePerDeviceState **)task->local_args);

  // get gate_pred, gate_assign, output
  auto gate_pred = acc.get_tensor<READ_WRITE>(GATE_PREDS);
  auto gate_assign = acc.get_tensor<READ_WRITE>(GATE_ASSIGN);
  auto output = acc.get_tensor<WRITE_ONLY>(OUTPUT);

  coord_t batch_size = gate_pred.shape[1];
  assert(batch_size == gate_assign.shape[1]);
  assert(gate_pred.shape[0] == gate_assign.shape[0]);
  assert(batch_size == output.shape[1]);
  coord_t out_dim = output.shape[0];

  // get exp_preds
  auto acc_exp_preds = acc.get_variadic_tensor<READ_WRITE>(EXP_PREDS);
  coord_t rows = acc_exp_preds[0].shape[1];
  assert (all_of(acc_exp_preds, [&](GenericTensorAccessorW const &a) { return a.shape[1] == rows; }));
  assert (all_of(acc_exp_preds, [&](GenericTensorAccessorW const &a) { return a.shape[0] == out_dim; }));
  
  std::vector<float *> exp_preds = vector_transform([](GenericTensorAccessorW const &a) { return a.get_float_ptr(); }, acc_exp_preds);
  assert (exp_preds.size() == n);

  int k = (int)(gate_assign.shape[0]);

  profile(
    forward_kernel, 
    m->profiling, 
    "[Aggregate] forward_time = %.2lfms\n",
    m,
    exp_preds.data(),
    gate_assign.get_float_ptr(),
    gate_pred.get_float_ptr(),
    output.get_float_ptr(),
    n,
    k,
    rows,
    batch_size,
    out_dim
  );
}

void Aggregate::backward(FFModel const &ff) {
  this->execute_task(ff, AGGREGATE_BWD_TASK_ID, get_bwd_task_signature());

  // ArgumentMap argmap;
  // Context ctx = ff.config.lg_ctx;
  // Runtime *runtime = ff.config.lg_hlr;
  // set_argumentmap_for_backward(ff, argmap);
  // IndexLauncher launcher(AGGREGATE_BWD_TASK_ID,
  //                        parallel_is,
  //                        TaskArgument(this, sizeof(Aggregate)),
  //                        argmap,
  //                        Predicate::TRUE_PRED,
  //                        false /*must*/,
  //                        0 /*mapper_id*/,
  //                        get_std_hash(outputs[0]->machine_view));
  // // gate_preds
  // launcher.add_region_requirement(RegionRequirement(inputs[0]->part,
  //                                                   0 /*projection id*/,
  //                                                   READ_WRITE,
  //                                                   EXCLUSIVE,
  //                                                   inputs[0]->region));
  // launcher.add_field(0, FID_DATA);
  // // gate_assign
  // launcher.add_region_requirement(RegionRequirement(inputs[1]->part,
  //                                                   0 /*projection id*/,
  //                                                   READ_ONLY,
  //                                                   EXCLUSIVE,
  //                                                   inputs[1]->region));
  // launcher.add_field(1, FID_DATA);
  // // true gate_assign
  // launcher.add_region_requirement(RegionRequirement(inputs[2]->part,
  //                                                   0 /*projection id*/,
  //                                                   READ_ONLY,
  //                                                   EXCLUSIVE,
  //                                                   inputs[2]->region));
  // launcher.add_field(2, FID_DATA);
  // // full_gate gradients
  // launcher.add_region_requirement(RegionRequirement(inputs[3]->part_grad,
  //                                                   0 /*projection id*/,
  //                                                   READ_WRITE,
  //                                                   EXCLUSIVE,
  //                                                   inputs[3]->region_grad));
  // launcher.add_field(3, FID_DATA);
  // // exp_preds
  // for (int i = 0; i < n; i++) {
  //   launcher.add_region_requirement(RegionRequirement(inputs[i + 4]->part,
  //                                                     0 /*projection id*/,
  //                                                     READ_WRITE,
  //                                                     EXCLUSIVE,
  //                                                     inputs[i + 4]->region));
  //   launcher.add_field(i + 4, FID_DATA);
  // }
  // // exp_preds gradients
  // for (int i = 0; i < n; i++) {
  //   launcher.add_region_requirement(
  //       RegionRequirement(inputs[i + 4]->part_grad,
  //                         0 /*projection id*/,
  //                         READ_WRITE,
  //                         EXCLUSIVE,
  //                         inputs[i + 4]->region_grad));
  //   launcher.add_field(i + n + 4, FID_DATA);
  // }

  // // output
  // launcher.add_region_requirement(RegionRequirement(outputs[0]->part_grad,
  //                                                   0 /*projection id*/,
  //                                                   READ_WRITE,
  //                                                   EXCLUSIVE,
  //                                                   outputs[0]->region_grad));
  // launcher.add_field(2 * n + 4, FID_DATA);

  // runtime->execute_index_space(ctx, launcher);
}


void Aggregate::backward_task(Task const *task,
                              std::vector<PhysicalRegion> const &regions,
                              Context ctx,
                              Runtime *runtime) {
  OpTaskArgumentAccessor acc(task, regions, ctx, runtime);

  AggregatePerDeviceState const *m = *((AggregatePerDeviceState **)task->local_args);
  
  auto const &attrs = acc.get_argument<AggregateAttrs>(ATTRS);

  int n = attrs.n;
  float lambda_bal = attrs.lambda_bal;

  assert((int)regions.size() == 2 * n + 5);
  assert((int)task->regions.size() == 2 * n + 5);

  // get gate_pred, gate_grad, gate_assign, output_grad
  auto gate_pred = acc.get_tensor<READ_ONLY>(GATE_PREDS);
  auto gate_assign = acc.get_tensor<READ_ONLY>(GATE_ASSIGN);
  auto true_gate_assign = acc.get_tensor<READ_ONLY>(TRUE_GATE_ASSIGN);
  auto full_gate_grad = acc.get_tensor_grad<READ_WRITE>(GATE_GRADIENTS_FULL);
  auto output_grad = acc.get_tensor_grad<READ_ONLY>(OUTPUT);

  coord_t batch_size = gate_pred.shape[1];
  assert(batch_size == gate_assign.shape[1]);
  assert(gate_assign.shape == true_gate_assign.shape);
  assert(batch_size == full_gate_grad.shape[1]);
  coord_t k = gate_assign.shape[0];
  assert(k * batch_size == output_grad.shape[1]);
  assert(gate_pred.shape[0] == k);
  coord_t out_dim = output_grad.shape[0];
  assert(n == full_gate_grad.shape[0]);

  // get exp_preds
  auto acc_exp_preds = acc.get_variadic_tensor<READ_WRITE>(EXP_PREDS);
  coord_t rows = acc_exp_preds[0].shape[1];
  assert (all_of(acc_exp_preds, [&](GenericTensorAccessorW const &a) { return a.shape[1] == rows; }));
  assert (all_of(acc_exp_preds, [&](GenericTensorAccessorW const &a) { return a.shape[0] == out_dim; }));
  
  std::vector<float *> exp_preds = vector_transform([](GenericTensorAccessorW const &a) { return a.get_float_ptr(); }, acc_exp_preds);
  assert (exp_preds.size() == n);

  // get chosen_exp_grads
  auto acc_exp_grads = acc.get_variadic_tensor_grad<READ_WRITE>(EXP_PREDS);
  
  size_t rows = acc_exp_grads[0].shape[1];
  assert (all_of(acc_exp_grads, [&](GenericTensorAccessorW const &a) { return a.shape[1] == rows; }));
  assert (all_of(acc_exp_grads, [&](GenericTensorAccessorW const &a) { return a.shape[0] == out_dim; }));

  std::vector<float *> exp_grads = vector_transform([](GenericTensorAccessorW const &a) { return a.get_float_ptr(); }, acc_exp_grads);
  assert (exp_grads.size() == n);

  /* profiling_wrapper() */

  profile(
    backward_kernel,
    m->profiling,
    "[Aggregate] backward_time = %.2lfms\n",
    m,
    exp_preds.data(), 
    exp_grads.data(),
    gate_assign.get_float_ptr(),
    true_gate_assign.get_float_ptr(),
    gate_pred.get_float_ptr(),
    full_gate_grad.get_float_ptr(),
    output_grad.get_float_ptr(),
    n,
    k,
    rows,
    lambda_bal,
    batch_size,
    out_dim
  );
}

/* void Aggregate::serialize(Legion::Serializer &sez) const { */
/*   sez.serialize(this->n); */
/*   sez.serialize(this->lambda_bal); */
/* } */

bool Aggregate::measure_operator_cost(Simulator *sim,
                                      MachineView const &mv,
                                      CostMetrics &cost_metrics) const {
  assert(numInputs <= MAX_NUM_INPUTS);
  ParallelTensorBase sub_inputs[MAX_NUM_INPUTS], sub_pred, sub_assign,
      sub_output;

  for (int i = 0; i < numInputs; ++i) {
    if (!inputs[i + 4]->get_sub_tensor(mv, sub_inputs[i])) {
      return false;
    }
  }
  if (!inputs[0]->get_sub_tensor(mv, sub_pred)) {
    return false;
  }
  if (!inputs[1]->get_sub_tensor(mv, sub_assign)) {
    return false;
  }

  if (!outputs[0]->get_sub_tensor(mv, sub_output)) {
    return false;
  }

  AggregatePerDeviceState *m = new AggregatePerDeviceState(sim->handler, attrs.n);

  // allocate
  sim->free_all();
  float *input_ptrs[MAX_NUM_INPUTS];
  bool out_of_memory = false;
  for (int i = 0; i < numInputs; ++i) {
    input_ptrs[i] =
        (float *)sim->allocate(sub_inputs[i].get_volume(), DT_FLOAT);
    out_of_memory = out_of_memory || (input_ptrs[i] == NULL);
  }
  int *assign_ptr = (int *)sim->allocate(sub_assign.get_volume(), DT_INT32);
  out_of_memory = out_of_memory || (assign_ptr == NULL);
  float *pred_ptr = (float *)sim->allocate(sub_pred.get_volume(), DT_FLOAT);
  out_of_memory = out_of_memory || (pred_ptr == NULL);
  cost_metrics.inputs_memory += cost_metrics.total_mem_diff_from(sim->offset);

  float *output_ptr = (float *)sim->allocate(sub_output.get_volume(), DT_FLOAT);
  cost_metrics.outputs_memory += cost_metrics.total_mem_diff_from(sim->offset);
  out_of_memory = out_of_memory || (output_ptr == NULL);

  if (out_of_memory) {
    cost_metrics.forward_time = Simulator::MAXIMUM_TASK_RUN_TIME;
    cost_metrics.backward_time = Simulator::MAXIMUM_TASK_RUN_TIME;
    return true;
  }

  assert(m->profiling == false);

  // compute
  std::function<void(ffStream_t)> forward, backward;
  Domain assign_domain = sub_assign.get_domain();
  Domain exp_domain = sub_inputs[0].get_domain();

  int k = assign_domain.hi()[0] - assign_domain.lo()[0] + 1;
  int batch_size = assign_domain.hi()[1] - assign_domain.lo()[1] + 1;
  int rows = exp_domain.hi()[1] - exp_domain.lo()[1] + 1;
  int out_dim = exp_domain.hi()[0] - exp_domain.lo()[0] + 1;

  forward = [&](ffStream_t stream) {
    forward_kernel(stream, m,
                           input_ptrs,
                           assign_ptr,
                           pred_ptr,
                           output_ptr,
                           attrs.n,
                           k,
                           rows,
                           batch_size,
                           out_dim);
  };

  inner_measure_operator_cost(sim, forward, backward, cost_metrics);
  log_measure.debug("[Measure Aggregate] name(%s) forward_time(%.4lf)\n",
                    name,
                    cost_metrics.forward_time);

  cost_metrics.backward_time = 0.0f; // not implemented for backward
  delete m;
  return true;
}

}
