#include "layer_5_attack_manager_without_average.hpp"
#include "../../../lib/sampling/sampler_primitives.hpp"
#include "../../../lib/sampling/samplers/average_sampler.hpp"
#include "../../../lib/sampling/samplers/list_sampler.hpp"

LayerAttackManagerWithoutAverage::LayerAttackManagerWithoutAverage(size_t first_index,
                                                                   size_t second_index,
                                                                   MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 5, "Attack manager"),
  temp_results_(),
  temp_attack_results_(),
  attack_manager_(std::move(memory_wrapper),
                  AttackType::FlushReload,
                  std::make_unique<ListSampler>(std::vector<size_t>{ first_index, second_index },
                                                static_cast<int>(DELAY),
                                                static_cast<int>(DELAY),
                                                std::make_shared<FlushSamplerPrimitive>()))
{
  attack_manager_.Calibrate();
}
std::vector<Measurement>& LayerAttackManagerWithoutAverage::Sample()
{
  attack_manager_.Attack(temp_results_, temp_attack_results_);

  return temp_results_;
}
void LayerAttackManagerWithoutAverage::Finalize()
{
  temp_results_.clear();
  temp_attack_results_.clear();
}
