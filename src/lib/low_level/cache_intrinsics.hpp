#pragma once
#include <cstdint>

namespace Memory
{
// Probe the time in cycles it take to access {p}
__attribute__((always_inline)) int32_t ProbeTiming(void* p)
{
  volatile int32_t time;

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
  int tmp;
  asm volatile("mov (%1), %0" : "+r"(tmp) : "r"(p) :);
}

// Flush the given address.
__attribute__((always_inline)) void MemoryFlush(void* p)
{
  asm volatile("clflush 0(%0)\n" : : "c"(p) :);
}
} // namespace Memory