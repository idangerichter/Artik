#include "attack_manager.hpp"
#include <stdexcept>
#include <utility>


AttackManager::AttackManager(MemoryWrapper&& memory_wrapper,
                             AttackType attack_type,
                             std::shared_ptr<Sampler> sampler,
                             std::shared_ptr<ScoreProvider> score_provider) :
AttackManager(std::move(memory_wrapper), attack_type, sampler)
{
  score_provider_ = score_provider;
}

AttackManager::AttackManager(MemoryWrapper&& memory_wrapper, AttackType attack_type, std::shared_ptr<Sampler> sampler) :
sampler_(std::move(sampler)),
attack_type_(attack_type),
memory_wrapper_(std::move(memory_wrapper))
{
}

void AttackManager::Calibrate()
{
  // FIXME
  std::vector<Measurement> measurements;
  score_provider_ = Calibration::Calibrate(measurements, attack_type_);
}
void AttackManager::Attack(std::vector<Measurement>& measurements, std::vector<AttackResult>& results)
{
  if (!score_provider_)
  {
    throw std::logic_error("Calibration was not called");
  }
  sampler_->Sample(memory_wrapper_, measurements);
  measurements.size();

  results.reserve(results.size() + sampler_->GetRequiredSize());

  for (const Measurement& measurement : measurements)
  {
    results.push_back(AttackResult{ measurement.index, score_provider_->Normalize(measurement) });
  }
}