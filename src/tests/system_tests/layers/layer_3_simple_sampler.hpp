#pragma once

#include "../../../lib/sampling/samplers/simple_sampler.hpp"
#include "layer.hpp"
class LayerSimpleSampler : public Layer
{
public:
  LayerSimpleSampler(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper);

  std::vector<Measurement>& Sample() override;
  void Finalize() override;

private:
  std::vector<Measurement> temp_results_;
  MemoryWrapper memory_wrapper_;
  SimpleSampler first_sampler_;
  SimpleSampler second_sampler_;

};
