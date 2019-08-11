#pragma once
#include "../sampler.hpp"
#include <thread>
/////////////////////////////// Average Sampler ///////////////////////////////
//  AverageSampler, uses any sampler and average out it results for a 
//  specified amount of rounds.
//  It does so in a way which is friendly for cache measurements by clearing,
//  measurements_vector of sampler_ after every round.
///////////////////////////////////////////////////////////////////////////////
class AverageSampler : public Sampler
{
public:
  explicit AverageSampler(std::unique_ptr<Sampler> sampler, size_t count, size_t between_rounds_delay);

  void Sample(MemoryWrapper& memory, std::vector<Measurement>& measurements_vector) override;
  size_t GetRequiredSize() const override;

private:
  const std::unique_ptr<Sampler> sampler_;
  const size_t between_rounds_delay_;
  const size_t count_;
  const size_t data_size_;

  // Due to performance issues, we allocate the vector once.
  std::vector<Measurement> temp_measurements_;
  std::vector<Measurement> temp_aggregating_measurements_;
};