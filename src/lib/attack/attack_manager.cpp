#include "attack_manager.hpp"
#include "../calibration/calibration.hpp"
#include <stdexcept>
#include <thread>
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

void AttackManager::Calibrate( size_t flushed_sample_rounds,
                              size_t action_sample_delay,
                              size_t between_samples_delay)
{
  std::vector<Measurement> measurements;
  measurements.reserve(flushed_sample_rounds);

  for (size_t i = 0; i < flushed_sample_rounds; i++)
  {
    memory_wrapper_.Flush(0);
    if (action_sample_delay != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(action_sample_delay));
    }
    measurements.push_back(Measurement{ 0, memory_wrapper_.Measure(0) });

    if (between_samples_delay != 0)
    {
      std::this_thread::sleep_for(std::chrono::nanoseconds(between_samples_delay));
    }
  }

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
