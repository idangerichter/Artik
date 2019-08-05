#pragma once
#include <cassert>


//  Create bitmask for {from, from+1, ..., to}
//  if from = to, it will mask {from} 
static constexpr uint64_t Bitmask(const unsigned int from, const unsigned int to)
{
    if (from == to)
    {
        return from == 0 ? 0 : static_cast<uint64_t>(1u << from);
    }
    else
    {
        return (static_cast<uint64_t>(-1) << from) & (static_cast<uint64_t>(-1) >> ((sizeof(uint64_t) * 8) - to - 1));
    }
}


// Extract the bits {from, from+1, ..., to} from field.
uint64_t ExtractBits(uint64_t field,  const unsigned int from, const unsigned int to)
{
    return (field & Bitmask(from, to)) >> from;
}

// Extract the bit {bit} from the field 
bool ExtractBit(uint64_t field, unsigned int const bit)
{
    return (field & (static_cast<uint64_t>(1) << bit)) != 0;
}
