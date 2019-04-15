#pragma once

#include "cpuid.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

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
    CacheType type;
    /**
     * The cache level, L1, L2 or L3 (1 based)
     */
    int32_t level;
    /**
     * Is the cache fully associative
     */
    bool fully_associative;
    /**
     * The cache line size, in bytes
     */
    int32_t line_size;
    /**
     * How many ways the cache have for each line
     */
    int32_t cache_associativity;
    /**
     * How many cache buckets there actually are (*not* multiplied by associativity)
     */
    int32_t sets_count;
    /**
     * Total size of cache, in bytes
     */
    int32_t total_cache_size;

    /**
     * Parse cpu id command to get cache information
     */
    explicit CacheInfo(const CPUID& cache_command);

    /**
     * Return the available cache info
     */
    static std::vector<CacheInfo> GetAll();
};


std::ostream& operator<<(std::ostream& stream, const CacheInfo& cacheInfo);
std::ostream& operator<<(std::ostream& stream, const CacheType& cacheType);
