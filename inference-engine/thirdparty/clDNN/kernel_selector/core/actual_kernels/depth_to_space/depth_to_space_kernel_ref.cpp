// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "depth_to_space_kernel_ref.h"
#include "kernel_selector_utils.h"
#include <string>
#include <vector>

namespace kernel_selector {

ParamsKey DepthToSpaceKernelRef::GetSupportedKey() const {
    ParamsKey k;
    k.EnableInputDataType(Datatype::F16);
    k.EnableInputDataType(Datatype::F32);
    k.EnableInputDataType(Datatype::UINT8);
    k.EnableInputDataType(Datatype::INT8);
    k.EnableOutputDataType(Datatype::F16);
    k.EnableOutputDataType(Datatype::F32);
    k.EnableOutputDataType(Datatype::UINT8);
    k.EnableOutputDataType(Datatype::INT8);
    k.EnableDifferentTypes();
    k.EnableAllInputLayout();
    k.EnableAllOutputLayout();
    k.EnableTensorOffset();
    k.EnableTensorPitches();
    k.EnableBatching();
    return k;
}

KernelsData DepthToSpaceKernelRef::GetKernelsData(const Params& params, const optional_params& options) const {
    return GetCommonKernelsData(params, options);
}

KernelsPriority DepthToSpaceKernelRef::GetKernelsPriority(const Params& /*params*/, const optional_params& /*options*/) const {
    return FORCE_PRIORITY_9;
}

JitConstants DepthToSpaceKernelRef::GetJitConstants(const depth_to_space_params& params) const {
    auto jit = Parent::GetJitConstants(params);
    auto input = params.inputs[0];
    auto input_dt = input.GetDType();

    if (!params.fused_ops.empty()) {
        std::vector<std::string> idx_order;
        if (input.Dimentions() == 5) {
            idx_order = {"batch", "feature", "z", "y", "x"};
        } else if (input.Dimentions() == 4) {
            idx_order = {"batch", "feature", "y", "x"};
        }
        FusedOpsConfiguration conf = {"", idx_order, "in_val", input_dt, 1};
        jit.Merge(MakeFusedOpsJitConstants(params, {conf}));
    }

    return jit;
}

}  // namespace kernel_selector
