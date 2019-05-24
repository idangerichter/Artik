#include <utility>

#include "samplers.hpp"
#include <thread>


SimpleSampler::SimpleSampler(size_t index, size_t delay, std::unique_ptr<SamplerPrimitive> sampler_primitive)
: index(index), delay(delay), sampler_primitive(std::move(sampler_primitive))
{
}

std::vector<Measurement> SimpleSampler::Sample(MemoryWrapper& memory) const
{
    sampler_primitive->Prepare(memory, index);
    std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
    Measurement measurement = sampler_primitive->Sample(memory, index);

    return { measurement };
}
ListSampler::ListSampler(std::vector<size_t> indexes,
                         size_t sample_measure_delay,
                         size_t between_items_delay,
                         std::unique_ptr<SamplerPrimitive> sampler_primitive)
: indexes(std::move(indexes)), sample_measure_delay(sample_measure_delay),
  between_items_delay(between_items_delay), sampler_primitive(std::move(sampler_primitive))
{
}

std::vector<Measurement> ListSampler::Sample(MemoryWrapper& memory) const
{
    std::vector<Measurement> results;
    for (auto const& index : indexes)
    {
        sampler_primitive->Prepare(memory, index);
        std::this_thread::sleep_for(std::chrono::nanoseconds(sample_measure_delay));
        Measurement measurement = sampler_primitive->Sample(memory, index);

        results.push_back(measurement);

        std::this_thread::sleep_for(std::chrono::nanoseconds(between_items_delay));
    }
    return results;
}
