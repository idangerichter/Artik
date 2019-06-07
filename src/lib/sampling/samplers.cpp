#include <utility>

#include "samplers.hpp"
#include <thread>


SimpleSampler::SimpleSampler(size_t index, size_t delay, std::unique_ptr<SamplerPrimitive> sampler_primitive)
: index(index), delay(delay), sampler_primitive(std::move(sampler_primitive))
{
}

void SimpleSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector)
{
    sampler_primitive->Prepare(memory, index);
    if (delay != 0) std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
    measurementsVector[0] = sampler_primitive->Sample(memory, index);
}

size_t SimpleSampler::GetRequiredSize() const
{
    return 1;
}

ListSampler::ListSampler(std::vector<size_t> indexes,
                         size_t sample_measure_delay,
                         size_t between_items_delay,
                         std::unique_ptr<SamplerPrimitive> sampler_primitive)
: indexes(std::move(indexes)), sample_measure_delay(sample_measure_delay),
  between_items_delay(between_items_delay), sampler_primitive(std::move(sampler_primitive))
{
}

void ListSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector)
{
    size_t i = 0;
    for (auto const& index : indexes)
    {
        sampler_primitive->Prepare(memory, index);

        if (sample_measure_delay != 0)
            std::this_thread::sleep_for(std::chrono::nanoseconds(sample_measure_delay));

        measurementsVector[i] = sampler_primitive->Sample(memory, index);

        if (between_items_delay != 0)
            std::this_thread::sleep_for(std::chrono::nanoseconds(between_items_delay));

        i++;
    }
}

size_t ListSampler::GetRequiredSize() const
{
    return indexes.size();
}
AverageSampler::AverageSampler(std::unique_ptr<Sampler> sampler, size_t count, size_t between_rounds_delay)
: sampler(std::move(sampler)), count(count), between_rounds_delay(between_rounds_delay),
  aggregate_vector(0)
{
    aggregate_vector = std::vector<Measurement>(this->sampler->GetRequiredSize());
}
size_t AverageSampler::GetRequiredSize() const
{
    return sampler->GetRequiredSize();
}
void AverageSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurementsVector)
{
    sampler->Sample(memory, aggregate_vector);
    for (size_t j = 0; j < aggregate_vector.size(); j++)
    {
        measurementsVector[j] = aggregate_vector[j];
    }

    for (size_t i = 0; i < count - 1; i++)
    {
        sampler->Sample(memory, aggregate_vector);
        for (size_t j = 0; j < aggregate_vector.size(); j++)
        {
            measurementsVector[j].time += aggregate_vector[j].time;
        }

        if (between_rounds_delay != 0)
            std::this_thread::sleep_for(std::chrono::nanoseconds(between_rounds_delay));
    }

    for (size_t j = 0; j < aggregate_vector.size(); j++)
    {
        measurementsVector[j].time /= count;
    }
}
