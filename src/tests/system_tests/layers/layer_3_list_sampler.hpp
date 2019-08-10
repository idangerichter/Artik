#pragma once

#include "../../../lib/sampling/samplers/list_sampler.hpp"
#include "layer.hpp"
class LayerListSampler : public Layer
{
public:
  LayerListSampler(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper);

  std::vector<Measurement>& Sample() override;
  void Finalize() override;

private:
  std::vector<Measurement> temp_results_;
  MemoryWrapper memory_wrapper_;
  ListSampler sampler_;
};
