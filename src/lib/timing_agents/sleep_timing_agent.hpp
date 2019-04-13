#pragma once

#include "timing_agent.hpp"

class SleepTimingAgent : public TimingAgent
{
public:
    SleepTimingAgent(const Board& board, int64_t sleep_micros);

    int64_t Measure(size_t i);

private:
    const Board& board_;
    int64_t sleep_millis_;
};