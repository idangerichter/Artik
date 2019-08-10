#include "layer_4_average_sampler.hpp"
#include "../../../lib/sampling/sampler_primitives.hpp"
#include "../../../lib/sampling/samplers/list_sampler.hpp"

LayerAverageSampler::LayerAverageSampler(size_t first_index,
                                         size_t second_index,
                                         MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 4, "Average sampler"),
  memory_wrapper_(std::move(memory_wrapper)),
  temp_results_(),
  sampler_(std::make_unique<ListSampler>(std::vector<size_t>{ first_index, second_index },
                                         static_cast<int>(DELAY),
                                         static_cast<int>(DELAY),
                                         std::make_shared<FlushSamplerPrimitive>()),
           COUNT,
           static_cast<int>(DELAY))
{
}
std::vector<Measurement>& LayerAverageSampler::Sample()
{
  sampler_.Sample(memory_wrapper_, temp_results_);

  return temp_results_;
}
void LayerAverageSampler::Finalize()
{
  temp_results_.clear();
}
