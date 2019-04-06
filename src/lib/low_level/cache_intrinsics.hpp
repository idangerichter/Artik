#pragma once
#include <cstdint>

/**
 * Provides the number of clock cycles since boot.
 */
uint64_t rdtsc();

/**
 * Probe the time in cycles it take to access {p}
 */
int64_t ProbeTiming(void* p);
/**
 * Access the address that was given.
 */
void MemoryAccess(void* p);

/**
 * Flush the given address.
 */
void MemoryFlush(void* p);
