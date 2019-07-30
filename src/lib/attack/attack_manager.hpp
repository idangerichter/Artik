#pragma once

#include "../calibration/calibration.hpp"
#include "../calibration/score_provider.hpp"
#include "../sampling/sampler.hpp"
#include <memory>


struct AttackResult
{
  size_t index;
  double score;
};

// This class is managing the attack
// It is in charge of giving calibration method
// and performing a cache attack, saving the results to attack_results_
//
// AttackManager has two modes, he either calibrate and generate
// ScoreProvider on his own, or he gets it from a outside source
class AttackManager
{
public:
  AttackManager(MemoryWrapper&& memory_wrapper,
                AttackType attack_type,
                std::unique_ptr<Sampler> sampler,
                std::shared_ptr<ScoreProvider> score_provider);

  AttackManager(MemoryWrapper&& memory_wrapper, AttackType attack_type,std::unique_ptr<Sampler> sampler);

  AttackManager(const AttackManager& attack) = delete;
  AttackManager(AttackManager&& attack) = default;
  AttackManager& operator=(const AttackManager& attack) = delete;

  // Generate score_provider_ using memory_wrapper_ and sampler_
  void Calibrate();

  // Perform a cache attack and append the results
  void Attack(std::vector<Measurement>& measurements, std::vector<AttackResult>& results);

private:
  AttackType attack_type_;
  MemoryWrapper memory_wrapper_;
  std::unique_ptr<Sampler> sampler_;
  std::shared_ptr<ScoreProvider> score_provider_;
};