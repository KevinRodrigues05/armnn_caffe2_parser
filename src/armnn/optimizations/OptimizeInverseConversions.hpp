//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#pragma once

#include "Optimization.hpp"

namespace armnn
{
namespace optimizations
{

class OptimizeInverseConversionsImpl
{
public:
    /// Run for every connection between two inverse data type conversion layers, i.e.
    /// Fp16ToFp32 followed by Fp32ToFp16 or vice-versa.
    void Run(Graph& graph, InputSlot& connection) const
    {
        Layer& base  = connection.GetConnectedOutputSlot()->GetOwningLayer();
        Layer& child = connection.GetOwningLayer();

        BOOST_ASSERT((base.GetType() == LayerType::ConvertFp16ToFp32 &&
                     child.GetType() == LayerType::ConvertFp32ToFp16) ||
                     (base.GetType() == LayerType::ConvertFp32ToFp16 &&
                     child.GetType() == LayerType::ConvertFp16ToFp32));

        // Bypass both conversion layers
        child.GetOutputSlot().MoveAllConnections(*base.GetInputSlot(0).GetConnectedOutputSlot());
    }

protected:
    OptimizeInverseConversionsImpl()  = default;
    ~OptimizeInverseConversionsImpl() = default;
};

using OptimizeInverseConversionsFp16 =
    OptimizeForConnection<ConvertFp16ToFp32Layer, ConvertFp32ToFp16Layer, OptimizeInverseConversionsImpl>;
using OptimizeInverseConversionsFp32 =
    OptimizeForConnection<ConvertFp32ToFp16Layer, ConvertFp16ToFp32Layer, OptimizeInverseConversionsImpl>;

} // namespace optimizations
} // namespace armnn
