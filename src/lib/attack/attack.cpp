#include "attack.hpp"
#include "../sampling/calibration.hpp"
#include <stdexcept>

Attack::Attack(MemoryWrapper& memoryWrapper,
               AttackType attackType,
               std::unique_ptr<Sampler> sampler,
               std::unique_ptr<ScoreProvider> scoreProvider)
: sampler(std::move(sampler)), scoreProvider(std::move(scoreProvider)), attackType(attackType),
  memoryWrapper(memoryWrapper)
{
}
Attack::Attack(MemoryWrapper& memoryWrapper, AttackType attackType, std::unique_ptr<Sampler> sampler)
: attackType(attackType), sampler(std::move(sampler)), memoryWrapper(memoryWrapper)
{
}

void Attack::calibrate()
{
    // FIXME
    Calibration calibration;
    std::vector<Measurement> measurements;
    scoreProvider = std::move(calibration.Calibrate(measurements, attackType));
}
std::vector<AttackResult> Attack::attack() const
{
    if (!scoreProvider)
    {
        throw std::logic_error("Calibration was not called");
    }
    std::vector<AttackResult> result;
    std::vector<Measurement> samples = sampler->Sample(memoryWrapper);
    for (const Measurement& measurement : samples)
    {
        AttackResult attackResult(measurement.index, scoreProvider->normalize(measurement));
        result.push_back(attackResult);
    }
    return result;
}

AttackResult::AttackResult(size_t index, double score) : index(index), score(score)
{
}
