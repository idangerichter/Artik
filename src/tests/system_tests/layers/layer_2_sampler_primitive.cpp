#include "layer_2_sampler_primitive.hpp"

LayerSamplerPrimitive::LayerSamplerPrimitive(size_t first_index,
                                             size_t second_index,
                                             MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 2, "Sampler primitive"),
  memory_wrapper_(std::move(memory_wrapper)),
  temp_results_(2),
  primitive_()
{
  temp_results_[0].index = first_index;
  temp_results_[1].index = second_index;
}
std::vector<Measurement>& LayerSamplerPrimitive::Sample()
{
  primitive_.Prepare(memory_wrapper_, first_index_);
  Sleep(DELAY);
  auto result1 = primitive_.Sample(memory_wrapper_, first_index_).time;

  Sleep(DELAY);

  primitive_.Prepare(memory_wrapper_, second_index_);
  Sleep(DELAY);
  auto result2 = primitive_.Sample(memory_wrapper_, second_index_).time;

  temp_results_[0].time = result1;
  temp_results_[1].time = result2;
  return temp_results_;
}
void LayerSamplerPrimitive::Finalize()
{
  // no-op
}
