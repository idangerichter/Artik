#pragma once

#include "../utils/memory_wrapper.hpp"
#include <vector>
/**
 * A record of measurement results
 */
struct Measurement
{
    size_t index;
    int32_t time;

    Measurement(size_t index, int32_t time);
};
/**
 * Sampler is using the SamplerPrimitive in a given pattern.
 * The responsibility of this class is to provide the data the real attack or the calibration needs.
 * It gets the measurements by repeatedly calling the SamplerPrimitive on selected indices.
 * There are few planned types of Samplers for each different use-cases.
 */
class Sampler
{
    public:
    /**
     * Perform a single sampling of the pattern, returning the results.
     * @param memory
     * @return
     */
    virtual std::vector<Measurement> Sample(MemoryWrapper& memory) const = 0;
};
