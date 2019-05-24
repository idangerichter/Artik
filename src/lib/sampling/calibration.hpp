#pragma once
#include "score_provider.hpp"
#include <memory>
#include <vector>

enum class AttackType : int
{
    FlushReload,
    PrimeProbe
};
class Calibration
{
    public:
    std::unique_ptr<ScoreProvider> Calibrate(std::vector<Measurement> measurements, AttackType type);
};