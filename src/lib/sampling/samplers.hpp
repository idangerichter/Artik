#pragma once
#include "sampler.hpp"
#include "sampler_primitive.hpp"


// Sampler that sample a single memory cell. It will wait `delay` nanoseconds between the preparation and sampling
class SimpleSampler : public Sampler
{
    public:
    explicit SimpleSampler(size_t index, size_t delay, SamplerPrimitive& sampler_primitive);

    std::vector<Measurement> Sample(MemoryWrapper& memory) const override;

    private:
    size_t index;
    size_t delay;
    SamplerPrimitive& sampler_primitive;
};

class ListSampler : public Sampler
{
    public:
    explicit ListSampler(std::vector<size_t> indexes,
                         size_t sample_measure_delay,
                         size_t between_items_delay,
                         SamplerPrimitive& sampler_primitive);

    std::vector<Measurement> Sample(MemoryWrapper& memory) const override;

    private:
    std::vector<size_t> indexes;
    size_t sample_measure_delay;
    size_t between_items_delay;
    SamplerPrimitive& sampler_primitive;
};
