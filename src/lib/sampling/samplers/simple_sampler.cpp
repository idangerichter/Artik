#include "simple_sampler.hpp"
#include <thread>

SimpleSampler::SimpleSampler(size_t index, size_t delay, std::shared_ptr<SamplerPrimitive> sampler_primitive)
: index_(index), delay_(delay), sampler_primitive_(sampler_primitive)
{
}

void SimpleSampler::Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector)
{
  measurements_vector.reserve(measurements_vector.size() + GetRequiredSize());

  sampler_primitive_->Prepare(memory, index_);

  if (delay_ != 0)
  {
    std::this_thread::sleep_for(std::chrono::nanoseconds(delay_));
  }

  measurements_vector.push_back(sampler_primitive_->Sample(memory, index_));
}

size_t SimpleSampler::GetRequiredSize() const
{
  return 1;
}
