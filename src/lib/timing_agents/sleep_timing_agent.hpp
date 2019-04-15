#pragma once

#include "timing_agent.hpp"

class SleepTimingAgent : public TimingAgent
{
public:
    SleepTimingAgent(const AttackSurface& surface, int64_t sleep_nano);

    int64_t Measure(size_t i);

private:
    const AttackSurface& surface_;
    int64_t sleep_nano_;
};