#include "sleep_timing_agent.hpp"
#include <chrono>
#include <thread>

SleepTimingAgent::SleepTimingAgent(const AttackSurface& surface, int64_t sleep_nano)
: surface_(surface), sleep_nano_(sleep_nano)
{
}

int64_t SleepTimingAgent::Measure(size_t i)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_nano_));
    return surface_.Measure(i);
}
