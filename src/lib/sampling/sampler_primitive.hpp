#pragma once

#include "../utils/memory_wrapper.hpp"
#include "measurement.hpp"
#include <cstddef>


// Responsible for accurately measuring the time to access memory while handling preparations and so on.
// For example:
//      Flush+Reload - The FlushReloadSampler flushes the given index in Prepare and access in Sample.
class SamplerPrimitive
{
public:
  // Prepare to perform the sampling on the given index.
  // @param memory The memory to execute the sampling on
  // @param index the index of the sampled cell
  virtual void Prepare(const MemoryWrapper& memory, size_t index) const;

  // Sample the access time and return the result.
  // Default implementation would be to call `memory.Measure(index)`.
  // @param memory The memory to execute the sampling on
  // @param index the index of the sampled cell
  virtual Measurement Sample(const MemoryWrapper& memory, size_t index) const;

  virtual ~SamplerPrimitive() = default;
};
