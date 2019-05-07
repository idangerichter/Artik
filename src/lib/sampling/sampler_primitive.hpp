#pragma once

#include "../utils/memory_wrapper.hpp"
#include <cstddef>
class SamplerPrimitive
{
    virtual void Prepare(MemoryWrapper& memory, size_t index) const = 0;
    virtual int32_t Sample(MemoryWrapper& memory, size_t index) const = 0;
};
