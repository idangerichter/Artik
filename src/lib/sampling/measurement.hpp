#pragma once

#include <cstddef>
#include <cstdint>
/**
 * A record of measurement results
 */
struct Measurement
{
    Measurement(size_t index, int32_t time);
    Measurement();
    
    size_t index;
    int32_t time;
};
