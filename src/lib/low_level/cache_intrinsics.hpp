#pragma once
#include <cstdint>

namespace Memory
{
// Provides the number of clock cycles since boot.
__attribute__((always_inline)) int64_t rdtsc()
{
    unsigned long long eax, edx;
    asm volatile("mfence");
    asm volatile("rdtsc" : "=a"(eax), "=d"(edx));
    eax = (edx << 32u) | eax;
    asm volatile("mfence");
    return eax;
}

// Probe the time in cycles it take to access {p}
__attribute__((always_inline)) int32_t ProbeTiming(void* p)
{
    volatile int64_t time;

    asm __volatile__("    mfence             \n"
                     "    lfence             \n"
                     "    rdtsc              \n"
                     "    lfence             \n"
                     "    movl %%eax, %%esi  \n"
                     "    movl (%1), %%eax   \n"
                     "    lfence             \n"
                     "    rdtsc              \n"
                     "    subl %%esi, %%eax  \n"
                     : "=a"(time)
                     : "c"(p)
                     : "%esi", "%edx");
    return static_cast<int32_t>(time);
}
// Access the address that was given.
__attribute__((always_inline)) void MemoryAccess(void* p)
{
    asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax");
}

// Flush the given address.
__attribute__((always_inline)) void MemoryFlush(void* p)
{
    asm volatile("clflush 0(%0)\n" : : "c"(p) : "rax");
}
} // namespace Memory