#pragma once
#include "../utils/Board.hpp"


/**
 *  a generic class for memory access measurement
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
     *  Perform a measurement on the ith cell in board
     *  returns the elapsed time.
     */
    virtual int64_t Measure(size_t i) = 0;

    virtual ~TimingAgent() = default;
private:
};