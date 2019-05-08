#pragma once

#include "../utils/memory_wrapper.hpp"
#include <cstddef>
/**
 * An attack primitive. Responsible for accurately measuring the time to access memory.
 * There is a different implementation for each attack since each requires different preparation
 */
class SamplerPrimitive
{
    public:
    /**
     * Prepare to perform the sampling on the given index. For example, FlushReloadSampler would flush the given index.
     * @param memory The memory to execute the sampling on
     * @param index the index of the sampled cell
     */
    virtual void Prepare(MemoryWrapper& memory, size_t index) const = 0;
    /**
     * Sample the access time and return the result. Default implementation would be to call `memory.Access(index)`,
     * but we provide the functionality to override it.
     * @param memory The memory to execute the sampling on
     * @param index the index of the sampled cell
     */
    virtual int32_t Sample(MemoryWrapper& memory, size_t index) const = 0;
};
