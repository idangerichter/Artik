#include "average_sampler.hpp"

AverageSampler::AverageSampler(std::shared_ptr<Sampler> sampler, size_t count, size_t between_rounds_delay) :
sampler_(sampler),
count_(count),
between_rounds_delay_(between_rounds_delay),
aggregate_vector_(sampler_->GetRequiredSize())
{
}

size_t AverageSampler::GetRequiredSize() const
{
  return sampler_->GetRequiredSize();
}
void AverageSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector)
{
  measurements_vector.reserve(measurements_vector.size() + GetRequiredSize());

  size_t first_index = measurements_vector.size();

  sampler_->Sample(memory, aggregate_vector_);
  for (size_t j = 0; j < aggregate_vector_.size(); j++)
  {
    measurements_vector.push_back(aggregate_vector_[j]);
  }

  for (size_t i = 0; i < count_ - 1; i++)
  {
    sampler_->Sample(memory, aggregate_vector_);
    for (size_t j = 0; j < aggregate_vector_.size(); j++)
    {
      measurements_vector[first_index + j].time += aggregate_vector_[j].time;
    }

    if (between_rounds_delay_ != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(between_rounds_delay_));
    }
  }

  for (size_t j = 0; j < aggregate_vector_.size(); j++)
  {
    measurements_vector[first_index + j].time /= count_;
  }
}
