#pragma once
#include "sampler.hpp"
#include "sampler_primitive.hpp"


/**
 * Sampler that sample a single memory cell. It will wait `delay` nanoseconds between the preparation and sampling
 */
class SimpleSampler : public Sampler
{
    public:
    std::vector<Measurement> Sample(MemoryWrapper& memory) const override;

    explicit SimpleSampler(size_t index, size_t delay, SamplerPrimitive& sampler_primitive);

    private:
    size_t index;
    size_t delay;
    SamplerPrimitive& sampler_primitive;
};

class ListSampler : public Sampler
{
    public:
    std::vector<Measurement> Sample(MemoryWrapper& memory) const override;
    explicit ListSampler(std::vector<size_t> indexes, size_t sample_measure_delay, size_t between_items_delay, SamplerPrimitive& sampler_primitive);

    private:
    std::vector<size_t> indexes;
    size_t sample_measure_delay;
    size_t between_items_delay;
    SamplerPrimitive& sampler_primitive;
};
