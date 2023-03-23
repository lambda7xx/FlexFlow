#ifndef _FLEXFLOW_OPS_KERNELS_CAST_KERNELS_H
#define _FLEXFLOW_OPS_KERNELS_CAST_KERNELS_H

#include "kernels/device.h"
#include "kernels/per_device_op_state.h"
#include "op-attrs/ffconst.h"
#include "kernels/accessor.h"

namespace FlexFlow {

class CastPerDeviceState : public PerDeviceOpState {
public:
  CastPerDeviceState(FFHandler handle);
  DataType input_data_type, output_data_type;
};

namespace Kernels {
namespace Cast {

void forward_kernel(ffStream_t stream,
                    CastPerDeviceState const *,
                    GenericTensorAccessorR const &input,
                    GenericTensorAccessorW const &output);

void backward_kernel(ffStream_t stream,
                     CastPerDeviceState const *,
                     GenericTensorAccessorR const &input,
                     GenericTensorAccessorW const &output);
        
}
}
}

#endif
