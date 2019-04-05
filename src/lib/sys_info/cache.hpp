#pragma once

#include "cpuid.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

/**
 * CPUID(4) outputs the following:


    EAX:
    31         26 25                    14 13    10  9  8 7    5 4        0
    +------------+------------------------+--------+--+--+------+----------+
    | cores/pkg  | threads sharing cache-1|########|FA|SI|Level |   Type   |
    +------------+------------------------+--------+--+--+------+----------+
    FA - Fully associative                                           |
    SI - Self-initializing                                           |
                                                                    |
    +---------------------------------------------------------------+
    |
    00H - no more cache information
    01H - Data cache
    02H - Instruction cache
    03H - Unified (data + instruction) cache
    EBX:
    31                 22 21                12 11                     0
    +--------------------+--------------------+------------------------+
    |  associativity - 1 | line partitions - 1|Coherency line size - 1 |
    +--------------------+--------------------+------------------------+
    ECX:
    31                                                               0
    +------------------------------------------------------------------+
    |                         Number of sets -1                        |
    +------------------------------------------------------------------+
 */


enum class CacheType : int
{
    UNDEFINED = 0,
    DATA = 1,
    INSTRUCTION = 2,
    UNIFIED = 3
};

struct CacheInfo
{
    /**
     * The type of the given cache: data cache, instruction cache or unified.
     */
    CacheType type; // EAX[0,4]
    /**
     * The cache level, L1, L2 or L3 (1 based)
     */
    int32_t level; // EAX[5,7]
    /**
     * Is the cache fully associative
     */
    bool fully_associative; // EAX[9]
    /**
     * The cache line size, in bytes
     */
    int32_t line_size; // EBX[0, 11]
    /**
     * How many ways the cache have for each line
     */
    int32_t cache_associativity; // EBX[22,31]
    /**
     * How many cache buckets there actually are (*not* multiplied by associativity)
     */
    int32_t sets_count; // ECX
    /**
     * Total size of cache, in bytes
     */
    int32_t total_cache_size; // line_size * cache_associativity * sets_count

    /**
     * Parse cpu id command to get cache information
     */
    explicit CacheInfo(const CPUID& cache_command);

    /**
     * Return the available cache info
     */
    static std::vector<CacheInfo> getAll();
};


std::ostream& operator<<(std::ostream& stream, const CacheInfo& cacheInfo);
std::ostream& operator<<(std::ostream& stream, const CacheType& cacheType);
