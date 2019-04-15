#pragma once
#include "../utils/attack_surface.hpp"


/**
 *  A generic class for memory access measurement
 *  different attacks may implement measurement differently due to
 *  attack specific constraints, e.g. - most attacks require some sleep
 *  between measurements.
 *  Using the generic timing agent, the integration
 *  of the different implementations into calibration and attack will be seamless.
 */

class TimingAgent
{
public:
    /**
     *  Performs a measurement on the ith cell in board
     *  returns the elapsed time.
     */
    virtual int64_t Measure(size_t i) = 0;

    virtual ~TimingAgent() = default;
private:
};