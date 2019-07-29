#pragma once
#include "../sampler.hpp"
#include "../sampler_primitive.hpp"

// ListSampler samples in a given indices set
class ListSampler : public Sampler
{
public:
  explicit ListSampler(const std::vector<size_t>& indices,
                       size_t sample_measure_delay,
                       size_t between_items_delay,
                       std::shared_ptr<SamplerPrimitive> sampler_primitive);

  void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector) override;
  size_t GetRequiredSize() const override;

private:
  const std::vector<size_t> indices_;
  const size_t sample_measure_delay_;
  const size_t between_items_delay_;
  const std::shared_ptr<SamplerPrimitive> sampler_primitive_;
};