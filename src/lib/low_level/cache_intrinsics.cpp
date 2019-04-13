#include "cache_intrinsics.hpp"

uint64_t rdtsc()
{
    unsigned long long a, d;
    asm volatile("mfence");
    asm volatile("rdtsc" : "=a"(a), "=d"(d));
    a = (d << 32) | a;
    asm volatile("mfence");
    return a;
}

void MemoryAccess(void* p)
{
    asm volatile("movq (%0), %%rax\n" : : "c"(p) : "rax");
}

void MemoryFlush(void* p)
{
    asm volatile("clflush 0(%0)\n" : : "c"(p) : "rax");
}

int64_t ProbeTiming(void* p)
{
    volatile long time;

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
    return time;
}
