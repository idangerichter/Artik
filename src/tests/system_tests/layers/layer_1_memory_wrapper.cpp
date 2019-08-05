#include "layer_1_memory_wrapper.hpp"

LayerMemoryWrapper::LayerMemoryWrapper(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 1, "Memory wrapper"),
  memory_wrapper_(std::move(memory_wrapper)),
  temp_results_(2)
{
  temp_results_[0].index = first_index;
  temp_results_[1].index = second_index;
}
std::vector<Measurement>& LayerMemoryWrapper::Sample()
{
  memory_wrapper_.Flush(first_index_);
  Sleep(DELAY);
  auto result1 = memory_wrapper_.Measure(first_index_);

  Sleep(DELAY);

  memory_wrapper_.Flush(second_index_);
  Sleep(DELAY);
  auto result2 = memory_wrapper_.Measure(second_index_);

  temp_results_[0].time = result1;
  temp_results_[1].time = result2;
  return temp_results_;
}
void LayerMemoryWrapper::Finalize()
{
  // no-op
}
