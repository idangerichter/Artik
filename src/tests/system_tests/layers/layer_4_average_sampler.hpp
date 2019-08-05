#pragma once

#include "../../../lib/sampling/samplers/average_sampler.hpp"
#include "layer.hpp"
class LayerAverageSampler : public Layer
{
public:
  LayerAverageSampler(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper);

  std::vector<Measurement>& Sample() override;
  void Finalize() override;

private:
  std::vector<Measurement> temp_results_;
  MemoryWrapper memory_wrapper_;
  AverageSampler sampler_;

};
