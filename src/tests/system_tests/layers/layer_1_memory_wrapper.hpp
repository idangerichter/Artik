#pragma once

#include "layer.hpp"
class LayerMemoryWrapper : public Layer
{
public:
  LayerMemoryWrapper(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper);

  std::vector<Measurement>& Sample() override;
  void Finalize() override;

private:
  std::vector<Measurement> temp_results_;
  MemoryWrapper memory_wrapper_;
};
