#include "simple_score_provider.hpp"
#include <stdexcept>

SimpleScoreProvider::SimpleScoreProvider(AttackType type, double threshold) :
  attack_type_(type),
  threshold_(threshold)
{
}
double SimpleScoreProvider::Normalize(Measurement measurement) const
{
  switch (attack_type_)
  {
  case AttackType::FlushReload:
    return measurement.time >= threshold_ ? 0.0 : 1.0;
  case AttackType::PrimeProbe:
    return measurement.time >= threshold_ ? 1.0 : 0.0;
  default:
    throw new std::range_error("Attack type should be FlushReload or PrimeProbe");
  }
}
