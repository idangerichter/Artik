#pragma once

#include "../sampling/calibration.hpp"
#include "../sampling/sampler.hpp"
#include "../sampling/score_provider.hpp"
#include <memory>

struct AttackResult
{
    size_t index;
    double score;

    AttackResult(size_t index, double score);
};

class Attack
{
    public:
    void calibrate();
    std::vector<AttackResult> attack() const;

    Attack(MemoryWrapper& memoryWrapper,
           AttackType attackType,
           std::unique_ptr<Sampler> sampler,
           std::unique_ptr<ScoreProvider> scoreProvider);
    Attack(MemoryWrapper& memoryWrapper, AttackType attackType, std::unique_ptr<Sampler> sampler);


    private:
    MemoryWrapper& memoryWrapper;
    AttackType attackType;
    std::unique_ptr<Sampler> sampler;
    std::unique_ptr<ScoreProvider> scoreProvider;
};