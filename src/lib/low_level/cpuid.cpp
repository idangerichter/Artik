#include "cpuid.hpp"

CPUID::CPUID(unsigned int command, unsigned int parameter)
{
    asm volatile("cpuid" : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3]) : "a"(command), "c"(parameter));
}

const uint32_t& CPUID::EAX() const
{
    return regs[0];
}
const uint32_t& CPUID::EBX() const
{
    return regs[1];
}
const uint32_t& CPUID::ECX() const
{
    return regs[2];
}
const uint32_t& CPUID::EDX() const
{
    return regs[3];
}
