#pragma once

#include <cstdint>
#include "../timing_agents/timing_agent.hpp"

static const int64_t DEFAULT_MEASURING_COUNT = 1000;
struct Thresholds
{
    int64_t victim_time;
    int64_t regular_time;
};

enum class VictimType
{
    VICTIM_SLOW,
    VICTIM_FAST
};

namespace Calibration
{
    Thresholds PassiveMeasurement(
            const TimingAgent& agent,
            VictimType victim_type = VictimType::VICTIM_SLOW,
            int64_t count = DEFAULT_MEASURING_COUNT);
    Thresholds ActiveMeasurement(
            const TimingAgent& regular_agent,
            const TimingAgent& victim_agent,
            int64_t count = DEFAULT_MEASURING_COUNT);
}