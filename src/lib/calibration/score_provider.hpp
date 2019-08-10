#pragma once
#include "../sampling/measurement.hpp"

class ScoreProvider
{
public:
  virtual double Normalize(Measurement measurement) const = 0;
};