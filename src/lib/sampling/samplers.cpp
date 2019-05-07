#include <utility>

#include "samplers.hpp"
#include <thread>
SimpleSampler::SimpleSampler(size_t index, size_t delay, SamplerPrimitive& sampler_primitive)
: index(index), delay(delay), sampler_primitive(sampler_primitive)
{
}

std::vector<Measurement> SimpleSampler::Sample(MemoryWrapper& memory) const
{
    sampler_primitive.Sample(memory, index);
    std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
    int32_t time = sampler_primitive.Sample(memory, index);

    Measurement measurement(index, time);
    return {measurement};
}
ListSampler::ListSampler(std::vector<size_t> indexes, size_t sample_measure_delay, size_t between_items_delay, SamplerPrimitive& sampler_primitive)
: indexes(std::move(indexes)), sample_measure_delay(sample_measure_delay), between_items_delay(between_items_delay),
  sampler_primitive(sampler_primitive)
{
}

std::vector<Measurement> ListSampler::Sample(MemoryWrapper& memory) const
{
    std::vector<Measurement> results;
    for (auto const& index : indexes)
    {
        sampler_primitive.Sample(memory, index);
        std::this_thread::sleep_for(std::chrono::nanoseconds(sample_measure_delay));
        int32_t time = sampler_primitive.Sample(memory, index);

        Measurement measurement(index, time);
        results.push_back(measurement);

        std::this_thread::sleep_for(std::chrono::nanoseconds(between_items_delay));
    }
    return results;
}
