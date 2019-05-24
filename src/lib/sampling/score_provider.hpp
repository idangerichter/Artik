#pragma once
#include "measurement.hpp"
#include <stdexcept>

class ScoreProvider {
public:
    virtual double normalize(Measurement measurement) const = 0;
};