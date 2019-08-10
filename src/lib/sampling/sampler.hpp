#pragma once
#include "../utils/memory_wrapper.hpp"
#include "measurement.hpp"
#include <vector>

// Sampler is using the SamplerPrimitive in a given pattern.
// It gets the measurements by repeatedly calling the SamplerPrimitive on selected indices.
class Sampler
{
public:
  // Perform a single sampling according to the pattern.
  // Append the results to measurements_vector.
  virtual void Sample(MemoryWrapper& memory,
                      std::vector<Measurement>& measurements_vector) = 0;

  // Returns the required size for the vector of the sampling.
  virtual size_t GetRequiredSize() const = 0;

  virtual ~Sampler() = default;
};
