#pragma once
#include "sleep_timing_agent.hpp"
#include <chrono>
#include <thread>

SleepTimingAgent::SleepTimingAgent(const Board& board, int64_t sleep_millis)
: board_(board), sleep_millis_(sleep_millis)
{
}

int64_t SleepTimingAgent::Measure(size_t i)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_millis_));
    return board_.Measure(i);
}
