#include "sampler_primitive.hpp"

void SamplerPrimitive::Prepare(MemoryWrapper& memory, size_t index) const
{
    // Default Prepare is no-op.
}

Measurement SamplerPrimitive::Sample(MemoryWrapper& memory, size_t index) const
{
    return {index, memory.Measure(index)};
}