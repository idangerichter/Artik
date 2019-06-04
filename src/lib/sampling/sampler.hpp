#pragma once
#include "../utils/memory_wrapper.hpp"
#include "measurement.hpp"
#include <vector>
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
     * @param memory Memory to operate on
     * @param measurementsVector vector to put the results on it.
     * @return list of measurements sorted by time of measurement
     */
    virtual void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector) = 0;
    /**
     * Returns the required size for the vector of the sampling
     */
    virtual size_t GetRequiredSize() const = 0;
};
