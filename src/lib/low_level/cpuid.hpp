#pragma once
#include <cstdint>

/* https://stackoverflow.com/a/4823889/4874829 */
class CPUID
{
private:
    uint32_t regs[4]{};

public:
    explicit CPUID(unsigned int command, unsigned int parameter = 0);

    const uint32_t& EAX() const;
    const uint32_t& EBX() const;
    const uint32_t& ECX() const;
    const uint32_t& EDX() const;
};