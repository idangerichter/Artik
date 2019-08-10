#include "layer_3_list_sampler.hpp"
#include "../../../lib/sampling/sampler_primitives.hpp"

LayerListSampler::LayerListSampler(size_t first_index,
                                   size_t second_index,
                                   MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 3, "List sampler"),
  memory_wrapper_(std::move(memory_wrapper)),
  temp_results_(),
  sampler_(std::vector<size_t>{ first_index, second_index },
           DELAY,
           DELAY,
           std::make_shared<FlushSamplerPrimitive>())
{
}
std::vector<Measurement>& LayerListSampler::Sample()
{
  sampler_.Sample(memory_wrapper_, temp_results_);

  return temp_results_;
}
void LayerListSampler::Finalize()
{
  temp_results_.clear();
}
