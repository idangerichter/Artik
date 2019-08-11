#pragma once
#include "../sampler.hpp"
#include "../sampler_primitive.hpp"

enum class ListSamplerMode
{
  Sequential,
  Parallel
};

///////////////////////////////// List Sampler ////////////////////////////////
//  ListSampler samples in a given indices set, in the given order.
//
//  It has two modes, Sequential and Parallel. 
//  - In sequential sampling, indices are prepared and measured one by one
//  - In parallel sampling, indicies are first prepared all at once and
//    then measured all at once.
///////////////////////////////////////////////////////////////////////////////
class ListSampler : public Sampler
{
public:
  explicit ListSampler(std::vector<size_t> indices,
                       size_t sample_measure_delay,
                       size_t between_items_delay,
                       std::shared_ptr<SamplerPrimitive> sampler_primitive,
                       ListSamplerMode mode = ListSamplerMode::Sequential);

  void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector) override;
  size_t GetRequiredSize() const override;

  ListSamplerMode mode_;

private:
  void SequentialSample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector);
  void ParallelSample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector);

  const std::vector<size_t> indices_;
  const size_t sample_measure_delay_;
  const size_t between_items_delay_;
  const std::shared_ptr<SamplerPrimitive> sampler_primitive_;
};