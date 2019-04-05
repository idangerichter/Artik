#pragma once
#include <assert.h>
/**
 * create bitmask for {from, from+1, ..., to}
 * if from == to the it will mask {from}
 */
template <typename R>
static constexpr R bitmask(unsigned int const from, unsigned int const to)
{
    if (from == to) {
        return from == 0 ? 0 : static_cast<R>(1 << from);
    } else {
        return (static_cast<R>(-1) << from) & (static_cast<R>(-1) >> ((sizeof(R) * 8) - to - 1));
    }
}

/**
 * Extract the bits {from, from+1, ..., to} from field.
 */ 
template <typename R>
static R extractBits(R field, unsigned int const from, unsigned int const to) {
    return (field & bitmask<R>(from, to)) >> from;
}

/**
 * Extract the bit {bit} from the field
 */
template <typename R>
static bool extractBit(R field, unsigned int const bit) {
    return (field & (static_cast<R>(1) << bit)) != 0;
}
