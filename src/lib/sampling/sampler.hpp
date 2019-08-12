#pragma once
#include "../utils/memory_wrapper.hpp"
#include "measurement.hpp"
#include <vector>

/////////////////////////////////// Sampler ///////////////////////////////////
//  Performs the logic of sampling in a given pattern using Sampler Primitive.
//
//  For performance and to avoid copying that can affect the cache and destroy 
//  measurements, the output is given via measurements_vector and not returned.
//  It is encouraged when using the sampler more than once, to clear 
//  measurements_vector and reuse it. This way avoids often reallocating, 
//  that may affect the credability of the measuremnts.
///////////////////////////////////////////////////////////////////////////////
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
