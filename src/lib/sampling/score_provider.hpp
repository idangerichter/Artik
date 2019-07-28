#pragma once
#include "measurement.hpp"

class ScoreProvider {
public:
    virtual double normalize(Measurement measurement) const = 0;
};