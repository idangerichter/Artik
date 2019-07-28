#pragma once

#include <cstddef>
#include <cstdint>

//  A record of measurement results 
struct Measurement
{
    explicit Measurement(size_t index = 0, int32_t time = 0);
    
    size_t index;
    int32_t time;
};
