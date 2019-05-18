#pragma once
#include <cassert>
/**
 * create bitmask for {from, from+1, ..., to}
 * if from == to the it will mask {from}
 */
static constexpr uint64_t Bitmask(unsigned int const from, unsigned int const to)
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

/**
 * Extract the bits {from, from+1, ..., to} from field.
 */
static uint64_t ExtractBits(uint64_t field, unsigned int const from, unsigned int const to)
{
    return (field & Bitmask(from, to)) >> from;
}

/**
 * Extract the bit {bit} from the field
 */
template <typename T> static bool ExtractBit(T field, unsigned int const bit)
{
    return (field & (static_cast<T>(1) << bit)) != 0;
}
