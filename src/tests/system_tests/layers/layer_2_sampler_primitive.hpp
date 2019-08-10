#pragma once

#include "../../../lib/sampling/sampler_primitives.hpp"
#include "layer.hpp"
class LayerSamplerPrimitive : public Layer
{
public:
  LayerSamplerPrimitive(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper);

  std::vector<Measurement>& Sample() override;
  void Finalize() override;

private:
  std::vector<Measurement> temp_results_;
  MemoryWrapper memory_wrapper_;
  FlushSamplerPrimitive primitive_;
};
