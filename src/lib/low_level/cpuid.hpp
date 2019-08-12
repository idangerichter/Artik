#pragma once
#include <cstdint>

class CPUID
{
public:
  explicit CPUID(unsigned int command, unsigned int parameter = 0);

  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
};