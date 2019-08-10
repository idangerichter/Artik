#include "sampler_primitive.hpp"

void SamplerPrimitive::Prepare(const MemoryWrapper& memory, size_t index) const
{
    // Default Prepare is no-op.
}

Measurement SamplerPrimitive::Sample(const MemoryWrapper& memory, size_t index) const
{
    return {index, memory.Measure(index)};
}