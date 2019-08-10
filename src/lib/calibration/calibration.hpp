#pragma once
#include "score_provider.hpp"
#include <memory>
#include <vector>

enum class AttackType
{
  FlushReload,
  PrimeProbe
};

// This namespace is in charge on doing analysis on the measurements
// and outputing a corresponding ScoreProvider
namespace Calibration
{
std::unique_ptr<ScoreProvider> Calibrate(std::vector<Measurement>& measurements_after_flush, AttackType type);
};