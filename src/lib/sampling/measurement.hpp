#pragma once

#include <cstddef>
#include <cstdint>

//  A record of measurement results
struct Measurement
{
  size_t index;
  int32_t time;

  bool operator==(const Measurement& rhs)
  {
    return index == rhs.index && time == rhs.time;
  }
};
