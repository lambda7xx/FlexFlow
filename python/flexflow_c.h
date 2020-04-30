#ifndef __FLEXFLOW_C_H__
#define __FLEXFLOW_C_H__

#include "ffconst.h"

#ifdef __cplusplus
extern "C" {
#endif
  
#define FF_NEW_OPAQUE_TYPE(T) typedef struct T { void *impl; } T

FF_NEW_OPAQUE_TYPE(flexflow_config_t);
FF_NEW_OPAQUE_TYPE(flexflow_model_t);
FF_NEW_OPAQUE_TYPE(flexflow_tensor_t);
FF_NEW_OPAQUE_TYPE(flexflow_sgd_optimizer_t);
FF_NEW_OPAQUE_TYPE(flexflow_glorot_uniform_initializer_t);
FF_NEW_OPAQUE_TYPE(flexflow_zero_initializer_t);
FF_NEW_OPAQUE_TYPE(flexflow_uniform_initializer_t);
FF_NEW_OPAQUE_TYPE(flexflow_norm_initializer_t);
FF_NEW_OPAQUE_TYPE(flexflow_op_t);
FF_NEW_OPAQUE_TYPE(flexflow_parameter_t);
FF_NEW_OPAQUE_TYPE(flexflow_net_config_t);
FF_NEW_OPAQUE_TYPE(flexflow_dataloader_t);

// -----------------------------------------------------------------------
// FFConfig
// -----------------------------------------------------------------------

flexflow_config_t
flexflow_config_create(void);

void
flexflow_config_destroy(
  flexflow_config_t handle);

void
flexflow_config_parse_args(
  flexflow_config_t handle,
  char** argv, 
  int argc);

void
flexflow_config_parse_args_default(
  flexflow_config_t handle);  

int
flexflow_config_get_batch_size(
  flexflow_config_t handle);

int
flexflow_config_get_workers_per_node(
  flexflow_config_t handle);

int
flexflow_config_get_num_nodes(
  flexflow_config_t handle);

int
flexflow_config_get_epochs(
  flexflow_config_t handle);

// -----------------------------------------------------------------------
// FFModel
// -----------------------------------------------------------------------

flexflow_model_t
flexflow_model_create(
  flexflow_config_t config);

void
flexflow_model_destroy(
  flexflow_model_t handle);

void
flexflow_model_reset_metrics(
  flexflow_model_t handle);

void
flexflow_model_init_layers(
  flexflow_model_t handle);

void
flexflow_model_prefetch(
  flexflow_model_t handle);

void
flexflow_model_forward(
  flexflow_model_t handle);

void
flexflow_model_backward(
  flexflow_model_t handle);

void
flexflow_model_update(
  flexflow_model_t handle);

void
flexflow_model_zero_gradients(
  flexflow_model_t handle);

flexflow_tensor_t
flexflow_model_add_conv2d(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  int out_channels,
  int kernel_h, int kernel_w,
  int stride_h, int stride_w,
  int padding_h, int padding_w,
  enum ActiMode activation /* AC_MODE_NONE */,
  bool use_bias /* True */);
  
flexflow_op_t
flexflow_model_add_conv2d_no_inout(
  flexflow_model_t handle,
  const char* name,
  int in_channels,
  int out_channels,
  int kernel_h, int kernel_w,
  int stride_h, int stride_w,
  int padding_h, int padding_w,
  enum ActiMode activation /* AC_MODE_NONE */,
  bool use_bias /* True */);

flexflow_tensor_t
flexflow_model_add_embedding_with_glorot_uniform_initializer(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  int num_entires, int out_dim,
  enum AggrMode aggr,
  flexflow_glorot_uniform_initializer_t kernel_initializer);  
  
flexflow_tensor_t
flexflow_model_add_embedding_with_zero_initializer(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  int num_entires, int out_dim,
  enum AggrMode aggr,
  flexflow_zero_initializer_t kernel_initializer);
  
flexflow_tensor_t
flexflow_model_add_embedding_with_uniform_initializer(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  int num_entires, int out_dim,
  enum AggrMode aggr,
  flexflow_uniform_initializer_t kernel_initializer);
  
flexflow_tensor_t
flexflow_model_add_embedding_with_norm_initializer(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  int num_entires, int out_dim,
  enum AggrMode aggr,
  flexflow_norm_initializer_t kernel_initializer);
  
flexflow_tensor_t
flexflow_model_add_pool2d(
  flexflow_model_t handle,
  const char* name,
  flexflow_tensor_t input,
  int kernel_h, int kernel_w,
  int stride_h, int stride_w,
  int padding_h, int padding_w,
  enum PoolType type /* POOL_MAX */, 
  enum ActiMode activation /* AC_MODE_NONE */);
  
flexflow_op_t
flexflow_model_add_pool2d_no_inout(
  flexflow_model_t handle,
  const char* name,
  int kernel_h, int kernel_w,
  int stride_h, int stride_w,
  int padding_h, int padding_w,
  enum PoolType type /* POOL_MAX */, 
  enum ActiMode activation /* AC_MODE_NONE */);

flexflow_tensor_t
flexflow_model_add_dense_with_default_initializer(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  int out_dim,
  enum ActiMode activation /* AC_MODE_NONE */,
  bool use_bias /* true */);
  
flexflow_op_t
flexflow_model_add_dense_with_default_initializer_no_inout(
  flexflow_model_t handle,
  const char* name,
  int in_dim,
  int out_dim,
  enum ActiMode activation /* AC_MODE_NONE */,
  bool use_bias /* true */);

flexflow_tensor_t
flexflow_model_add_concat(
  flexflow_model_t handle,
  const char* name,
  int n,
  flexflow_tensor_t* input,
  int axis);
  
flexflow_tensor_t
flexflow_model_add_flat(
  flexflow_model_t handle,
  const char* name,
  flexflow_tensor_t input);
  
flexflow_op_t
flexflow_model_add_flat_no_inout(
  flexflow_model_t handle,
  const char* name);
  
flexflow_tensor_t
flexflow_model_add_softmax(
  flexflow_model_t handle,
  const char* name,
  const flexflow_tensor_t input,
  const flexflow_tensor_t label);
  
void
flexflow_model_set_sgd_optimizer(
  flexflow_model_t handle, 
  flexflow_sgd_optimizer_t optimizer);
  
void
flexflow_model_print_layers(
  flexflow_model_t handle, 
  int id);

flexflow_op_t
flexflow_model_get_layer_by_id(
  flexflow_model_t handle,
  int layer_id);
  
flexflow_tensor_t
flexflow_model_get_tensor_by_id(
  flexflow_model_t handle,
  int layer_id);

// -----------------------------------------------------------------------
// Tensor
// -----------------------------------------------------------------------

flexflow_tensor_t
flexflow_tensor_4d_create(
  flexflow_model_t model,
  const int* dims, 
  const char* pc_name, 
  enum DataType data_type, 
  bool create_grad /* true */);

flexflow_tensor_t
flexflow_tensor_2d_create(
  flexflow_model_t model,
  const int* dims, 
  const char* pc_name, 
  enum DataType data_type, 
  bool create_grad /* true */);

void
flexflow_tensor_destroy(
  flexflow_tensor_t handle);

void
flexflow_tensor_inline_map(
  flexflow_tensor_t handle,
  flexflow_config_t config);

void  
flexflow_tensor_inline_unmap(
  flexflow_tensor_t handle,
  flexflow_config_t config);

float*  
flexflow_tensor_get_raw_ptr_float(
  flexflow_tensor_t handle,
  flexflow_config_t config);
  
int32_t*  
flexflow_tensor_get_raw_ptr_int32(
  flexflow_tensor_t handle,
  flexflow_config_t config);

int
flexflow_tensor_get_num_dims(
  flexflow_tensor_t handle);

int*
flexflow_tensor_get_dims(
  flexflow_tensor_t handle);

void
flexflow_tensor_attach_raw_ptr(
  flexflow_tensor_t handle,
  flexflow_config_t config,
  uintptr_t ptr,
  bool column_major);

void
flexflow_tensor_detach_raw_ptr(
  flexflow_tensor_t handle,
  flexflow_config_t config);
  
// -----------------------------------------------------------------------
// SGDOptimizer
// -----------------------------------------------------------------------

flexflow_sgd_optimizer_t
flexflow_sgd_optimizer_create(
  flexflow_model_t model,
  double lr, /* 0.01f */
  double momentum, /* 0.0f */
  bool nesterov, /* false */
  double weight_decay /* 0.0f */ );

void 
flexflow_sgd_optimizer_destroy(
  flexflow_sgd_optimizer_t handle);

// -----------------------------------------------------------------------
// GlorotUniform
// -----------------------------------------------------------------------

flexflow_glorot_uniform_initializer_t
flexflow_glorot_uniform_initializer_create(
  int seed);

void  
flexflow_glorot_uniform_initializer_destroy(
  flexflow_glorot_uniform_initializer_t handle);

// -----------------------------------------------------------------------
// ZeroInitializer
// -----------------------------------------------------------------------

flexflow_zero_initializer_t
flexflow_zero_initializer_create(void);

void  
flexflow_zero_initializer_destroy(
  flexflow_zero_initializer_t handle);

// -----------------------------------------------------------------------
// UniformInitializer
// -----------------------------------------------------------------------

flexflow_uniform_initializer_t
flexflow_uniform_initializer_create(
  int seed, 
  float min, 
  float max);

void  
flexflow_uniform_initializer_destroy(
  flexflow_uniform_initializer_t handle);

// -----------------------------------------------------------------------
// NormInitializer
// -----------------------------------------------------------------------

flexflow_norm_initializer_t
flexflow_norm_initializer_create(
  int seed, 
  float mean, 
  float stddev);

void  
flexflow_norm_initializer_destroy(
  flexflow_norm_initializer_t handle);

// -----------------------------------------------------------------------
// NetConfig
// -----------------------------------------------------------------------

flexflow_net_config_t
flexflow_net_config_create();

void
flexflow_net_config_destroy(
  flexflow_net_config_t handle);

const char*
flexflow_net_config_get_dataset_path(
  flexflow_net_config_t handle);

// -----------------------------------------------------------------------
// DataLoader
// -----------------------------------------------------------------------

flexflow_dataloader_t
flexflow_dataloader_create(
  flexflow_model_t ffmodel, 
  flexflow_net_config_t netconfig,
  flexflow_tensor_t input, 
  flexflow_tensor_t label);
  
void  
flexflow_dataloader_destroy(
  flexflow_dataloader_t handle);

void
flexflow_dataloader_set_num_samples(
  flexflow_dataloader_t handle,
  int samples);
  
int
flexflow_dataloader_get_num_samples(
  flexflow_dataloader_t handle);

void
flexflow_dataloader_reset(
  flexflow_dataloader_t handle);

void
flowflow_dataloader_next_batch(
  flexflow_dataloader_t handle,
  flexflow_model_t ffmodel);

// -----------------------------------------------------------------------
// Timer
// -----------------------------------------------------------------------

double
flexflow_get_current_time(
  flexflow_config_t config);

// -----------------------------------------------------------------------
// Trace
// -----------------------------------------------------------------------

void
flexflow_begin_trace(
  flexflow_config_t config, 
  int trace_id);

void
flexflow_end_trace(
  flexflow_config_t config, 
  int trace_id);
  
// -----------------------------------------------------------------------
// Op
// -----------------------------------------------------------------------

flexflow_tensor_t
flexflow_op_get_weight(
  flexflow_op_t handle); 
    
flexflow_tensor_t
flexflow_op_get_bias(
  flexflow_op_t handle); 

flexflow_tensor_t
flexflow_op_get_input_by_id(
  flexflow_op_t handle,
  int id); 
  
flexflow_tensor_t
flexflow_op_get_output(
  flexflow_op_t handle); 

void
flexflow_op_init(
  flexflow_op_t handle,
  flexflow_model_t model);
  
flexflow_tensor_t
flexflow_op_init_inout(
  flexflow_op_t handle,
  flexflow_model_t model,
  flexflow_tensor_t input);

void
flexflow_op_forward(
  flexflow_op_t handle,
  flexflow_model_t model);
  
void
flexflow_op_add_to_model(
  flexflow_op_t handle,
  flexflow_model_t model);
  

// -----------------------------------------------------------------------
// Parameter
// -----------------------------------------------------------------------

flexflow_tensor_t
flexflow_parameter_get_tensor(
  flexflow_parameter_t handle);  
  
int*
flexflow_malloc_int(
  int size);

void
flexflow_print_array_int(
  int *base_ptr,
  int size);

#ifdef __cplusplus
}
#endif

#endif // __FLEXFLOW_C_H__