#include "samplers.h"

void FlushSamplerPrimitive::Prepare(MemoryWrapper& memory, size_t index) const
{
    memory.Flush(index);
}

void LoadSamplerPrimitive::Prepare(MemoryWrapper& memory, size_t index) const
{
    memory.Access(index);
}
