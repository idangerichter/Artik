#include "average_sampler.hpp"
#include <utility>

AverageSampler::AverageSampler(std::unique_ptr<Sampler> sampler, size_t count, size_t between_rounds_delay) :
  sampler_(std::move(sampler)),
  between_rounds_delay_(between_rounds_delay),
  count_(count),
  data_size_(sampler_->GetRequiredSize()),
  temp_measurements_(data_size_),
  temp_aggregating_measurements_(data_size_)
{
}

size_t AverageSampler::GetRequiredSize() const
{
  return sampler_->GetRequiredSize();
}
void AverageSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector)
{
  // Run once to get right indices
  temp_measurements_.clear();
  sampler_->Sample(memory, temp_measurements_);

  for (size_t i = 0; i < data_size_; ++i)
  {
    temp_aggregating_measurements_[i] = temp_measurements_[i];
  }

  // Run the rest
  for (size_t round = 1; round < count_; ++round)
  {
    temp_measurements_.clear();
    sampler_->Sample(memory, temp_measurements_);

    for (size_t i = 0; i < data_size_; ++i)
    {
      temp_aggregating_measurements_[i].time += temp_measurements_[i].time;
    }

    if (between_rounds_delay_ != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(between_rounds_delay_));
    }
  }

  for (auto& measurement : temp_aggregating_measurements_)
  {
    measurement.time /= count_;
    measurements_vector.push_back(measurement);
  }
}