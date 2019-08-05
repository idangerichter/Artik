#include "list_sampler.hpp"
#include <thread>
#include <utility>

ListSampler::ListSampler(std::vector<size_t>  indices,
                         size_t sample_measure_delay,
                         size_t between_items_delay,
                         std::shared_ptr<SamplerPrimitive> sampler_primitive) :
indices_(std::move(indices)),
sample_measure_delay_(sample_measure_delay),
between_items_delay_(between_items_delay),
sampler_primitive_(std::move(sampler_primitive))
{
}

void ListSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector)
{
  measurements_vector.reserve(measurements_vector.size() + GetRequiredSize());

  for (auto const& index : indices_)
  {
    sampler_primitive_->Prepare(memory, index);

    if (sample_measure_delay_ != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(sample_measure_delay_));
    }

    measurements_vector.push_back(sampler_primitive_->Sample(memory, index));

    if (between_items_delay_ != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(between_items_delay_));
    }
  }
}

size_t ListSampler::GetRequiredSize() const
{
  return indices_.size();
}