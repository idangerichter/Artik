#include "layer_3_simple_sampler.hpp"
#include "../../../lib/sampling/sampler_primitives.hpp"

LayerSimpleSampler::LayerSimpleSampler(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 3, "Simple sampler"),
  memory_wrapper_(std::move(memory_wrapper)),
  temp_results_(),
  first_sampler_(first_index, DELAY, std::make_shared<FlushSamplerPrimitive>()),
  second_sampler_(second_index, DELAY, std::make_shared<FlushSamplerPrimitive>())
{
}
std::vector<Measurement>& LayerSimpleSampler::Sample()
{
  first_sampler_.Sample(memory_wrapper_, temp_results_);
  Sleep(DELAY);
  second_sampler_.Sample(memory_wrapper_, temp_results_);

  return temp_results_;
}
void LayerSimpleSampler::Finalize()
{
  temp_results_.clear();
}
