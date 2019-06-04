#pragma once
#include "sampler.hpp"
#include "sampler_primitive.hpp"
#include <memory>

// Sampler that sample a single memory cell. It will wait `delay` nanoseconds between the preparation and sampling
class SimpleSampler : public Sampler
{
    public:
    explicit SimpleSampler(size_t index, size_t delay, std::unique_ptr<SamplerPrimitive> sampler_primitive);

    void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector) override;
    size_t GetRequiredSize() const override;

    private:
    size_t index;
    size_t delay;
    std::unique_ptr<SamplerPrimitive> sampler_primitive;
};

class ListSampler : public Sampler
{
    public:
    explicit ListSampler(std::vector<size_t> indexes,
                         size_t sample_measure_delay,
                         size_t between_items_delay,
                         std::unique_ptr<SamplerPrimitive> sampler_primitive);

    void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector) override;
    size_t GetRequiredSize() const override;

    private:
    const std::vector<size_t> indexes;
    const size_t sample_measure_delay;
    const size_t between_items_delay;
    const std::unique_ptr<SamplerPrimitive> sampler_primitive;
};

class AverageSampler : public Sampler
{
    public:
    explicit AverageSampler(std::unique_ptr<Sampler> sampler, size_t count, size_t between_rounds_delay);

    void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector) override;
    size_t GetRequiredSize() const override;

    private:
    const std::unique_ptr<Sampler> sampler;
    const size_t between_rounds_delay;
    const size_t count;
    std::vector<Measurement> aggregate_vector;
};