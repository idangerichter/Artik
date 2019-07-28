#include <utility>

#include "../sampling/calibration.hpp"
#include "attack.hpp"
#include <stdexcept>

Attack::Attack(MemoryWrapper&& memoryWrapper,
               AttackType attackType,
               std::unique_ptr<Sampler> sampler,
               std::unique_ptr<ScoreProvider> scoreProvider)
: sampler(std::move(sampler)),
  scoreProvider(std::move(scoreProvider)),
  attackType(attackType),
  memoryWrapper(std::move(memoryWrapper)),
  attackResults(),
  samples()
{
    attackResults = std::vector<AttackResult>(this->sampler->GetRequiredSize());
    samples = std::vector<Measurement>(this->sampler->GetRequiredSize());
}
Attack::Attack(MemoryWrapper&& memoryWrapper, AttackType attackType, std::unique_ptr<Sampler> sampler)
: attackType(attackType),
  sampler(std::move(sampler)),
  memoryWrapper(std::move(memoryWrapper)),
  attackResults(),
  samples()
{
    attackResults = std::vector<AttackResult>(this->sampler->GetRequiredSize());
    samples = std::vector<Measurement>(this->sampler->GetRequiredSize());
}

void Attack::calibrate()
{
    // FIXME
    Calibration calibration;
    std::vector<Measurement> measurements;
    scoreProvider = std::move(calibration.Calibrate(measurements, attackType));
}
void Attack::attack()
{
    if (!scoreProvider)
    {
        throw std::logic_error("Calibration was not called");
    }
    sampler->Sample(memoryWrapper, samples);
    size_t i = 0;
    for (const Measurement& measurement : samples)
    {
        attackResults[i] = AttackResult(measurement.index, scoreProvider->normalize(measurement));
        i++;
    }
}

AttackResult::AttackResult(size_t index, double score) : index(index), score(score)
{
}

AttackResult::AttackResult() : index(0), score(0)
{
}