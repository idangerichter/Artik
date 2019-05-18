#include "cpuid.hpp"

CPUID::CPUID(unsigned int command, unsigned int parameter)
{
    asm volatile("cpuid"
                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                 : "a"(command), "c"(parameter));
}
