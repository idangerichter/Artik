#pragma once
#include "../sampler.hpp"
#include "../sampler_primitive.hpp"

// Sampler that sample a single memory cell.
// It will wait `delay` nanoseconds between the preparation and sampling
class SimpleSampler : public Sampler
{
public:
  explicit SimpleSampler(size_t index, size_t delay, std::shared_ptr<SamplerPrimitive> sampler_primitive);

  void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector) override;
  size_t GetRequiredSize() const override;

private:
  size_t index_;
  size_t delay_;
  std::shared_ptr<SamplerPrimitive> sampler_primitive_;
};
