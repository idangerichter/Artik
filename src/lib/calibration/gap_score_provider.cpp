#include "gap_score_provider.hpp"

GapScoreProvider::GapScoreProvider(AttackType type, double low_threshold, double high_threshold) :
  attack_type_(type),
  low_threshold_(low_threshold),
  high_threshold_(high_threshold)
{
}

double GapScoreProvider::Normalize(Measurement measurement) const
{
  double score;
  if (measurement.time < low_threshold_)
  {
    score = 1;
  }
  else if (measurement.time > high_threshold_)
  {
    score = 0;
  }
  else
  {
    score = 0.5;
  }

  switch (attack_type_)
  {
  case AttackType::FlushReload:
    return score;
  case AttackType::PrimeProbe:
    return 1.0 - score;
  default:
    throw std::range_error("Attack type should be FlushReload or PrimeProbe");
  }
}
