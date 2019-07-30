#include "average_sampler.hpp"

#include <utility>

AverageSampler::AverageSampler(std::unique_ptr<Sampler> sampler, size_t count, size_t between_rounds_delay) :
  sampler_(std::move(sampler)),
  between_rounds_delay_(between_rounds_delay),
  count_(count),
  temp_measurements_(sampler_->GetRequiredSize()),
  temp_aggregating_measurements_(sampler_->GetRequiredSize())
{
}

size_t AverageSampler::GetRequiredSize() const
{
  return sampler_->GetRequiredSize();
}
void AverageSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector)
{
  measurements_vector.reserve(measurements_vector.size() + GetRequiredSize());
  size_t size = GetRequiredSize();

  sampler_->Sample(memory, temp_aggregating_measurements_);

  for (size_t i = 0; i < count_ - 1; ++i)
  {
    sampler_->Sample(memory, temp_measurements_);

    for (size_t j = 0; j < size; ++j)
    {
      temp_aggregating_measurements_[j].time += temp_measurements_[j].time;
    }

    temp_measurements_.clear();

    if (between_rounds_delay_ != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(between_rounds_delay_));
    }
  }

  for (auto& sample : temp_aggregating_measurements_)
  {
    sample.time /= count_;
    measurements_vector.push_back(sample);
  }
}
