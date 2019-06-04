#include "measurement.hpp"

Measurement::Measurement(size_t index, int32_t time) : index(index), time(time)
{
}

Measurement::Measurement(): index(0), time(0)
{
    
}