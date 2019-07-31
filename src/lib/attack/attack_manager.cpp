#include "attack_manager.hpp"
#include <stdexcept>
#include <utility>


AttackManager::AttackManager(MemoryWrapper&& memory_wrapper,
                             AttackType attack_type,
                             std::unique_ptr<Sampler> sampler,
                             std::shared_ptr<ScoreProvider> score_provider) :
  AttackManager(std::move(memory_wrapper), attack_type, std::move(sampler))
{
  score_provider_ = std::move(score_provider);
}

AttackManager::AttackManager(MemoryWrapper&& memory_wrapper, AttackType attack_type, std::unique_ptr<Sampler> sampler) :
  attack_type_(attack_type),
  memory_wrapper_(std::move(memory_wrapper)),
  sampler_(std::move(sampler))
{
}

void AttackManager::Calibrate()
{
  // FIXME measure few times and pass to calibrate to process it
  // currently calibrate is no-op so this method is also no-op
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

  for (const Measurement& measurement : measurements)
  {
    results.push_back(AttackResult{ measurement.index, score_provider_->Normalize(measurement) });
  }
}
