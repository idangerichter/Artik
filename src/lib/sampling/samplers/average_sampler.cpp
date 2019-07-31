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
  size_t size = GetRequiredSize();

  for (size_t i = 0; i < count_; ++i)
  {
    temp_measurements_.clear();
    sampler_->Sample(memory, temp_measurements_);

    for (size_t j = 0; j < size; ++j)
    {
      temp_aggregating_measurements_[j].time += temp_measurements_[j].time;
    }

    if (between_rounds_delay_ != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(between_rounds_delay_));
    }
  }

  for (size_t i = 0; i < size; i++)
  {
    measurements_vector.push_back(Measurement{ i, static_cast<int32_t>(temp_aggregating_measurements_[i].time / count_) });
  }
}
