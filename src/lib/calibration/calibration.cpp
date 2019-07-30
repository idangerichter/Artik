#include "calibration.hpp"
#include "simple_score_provider.cpp"
#include <iostream>

constexpr double THRESHOLD = 160;

std::unique_ptr<ScoreProvider> Calibration::Calibrate(const std::vector<Measurement>& measurements, AttackType type)
{
  // TODO: anything in here ????
  return std::make_unique<SimpleScoreProvider>(type,THRESHOLD);
}
