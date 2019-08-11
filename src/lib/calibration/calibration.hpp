#pragma once
#include "score_provider.hpp"
#include <memory>
#include <vector>

enum class AttackType
{
  FlushReload,
  PrimeProbe
};

///////////////////////////////// Calibration /////////////////////////////////
//  This namespace is in charge on doing analysis on the measurements
//  and outputing a corresponding ScoreProvider.
//  It normalize the calibration results depending of the specific attack.
///////////////////////////////////////////////////////////////////////////////
namespace Calibration
{
std::unique_ptr<ScoreProvider> Calibrate(std::vector<Measurement>& measurements_after_flush, AttackType type);
};