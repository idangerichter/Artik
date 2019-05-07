#pragma once
#include <cassert>
/**
 * create bitmask for {from, from+1, ..., to}
 * if from == to the it will mask {from}
 */
template <typename T> static constexpr T Bitmask(unsigned int const from, unsigned int const to)
{
    if (from == to)
    {
        return from == 0 ? 0 : static_cast<T>(1u << from);
    }
    else
    {
        return (static_cast<T>(-1) << from) & (static_cast<T>(-1) >> ((sizeof(T) * 8) - to - 1));
    }
}

/**
 * Extract the bits {from, from+1, ..., to} from field.
 */
template <typename T> static T ExtractBits(T field, unsigned int const from, unsigned int const to)
{
    return (field & Bitmask<T>(from, to)) >> from;
}

/**
 * Extract the bit {bit} from the field
 */
template <typename T> static bool ExtractBit(T field, unsigned int const bit)
{
    return (field & (static_cast<T>(1) << bit)) != 0;
}
