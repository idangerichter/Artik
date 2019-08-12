#include "sampler_primitives.hpp"

void FlushSamplerPrimitive::Prepare(const MemoryWrapper& memory, size_t index) const
{
  memory.Flush(index);
}

void LoadSamplerPrimitive::Prepare(const MemoryWrapper& memory, size_t index) const
{
  memory.Access(index);
}
