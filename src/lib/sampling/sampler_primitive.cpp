#include "sampler_primitive.hpp"

void SamplerPrimitive::Prepare(MemoryWrapper& memory, size_t index) const
{
    // no-op
}

int32_t SamplerPrimitive::Sample(MemoryWrapper& memory, size_t index) const
{
    return memory.Measure(index);
}