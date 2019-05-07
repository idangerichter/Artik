#include "cache.hpp"
#include "../utils/bits.hpp"
#include <iostream>
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
static const unsigned int CPUID_CACHE_COMMAND = 4;

CacheInfo::CacheInfo(const CPUID& c)
: type(static_cast<CacheType>(ExtractBits(c.EAX(), 0, 4))), level(ExtractBits(c.EAX(), 5, 7)),
  fully_associative(ExtractBit(c.EAX(), 9)), line_size(ExtractBits(c.EBX(), 0, 11) + 1),
  cache_associativity(ExtractBits(c.EBX(), 22, 31) + 1), sets_count(c.ECX() + 1)
{
    total_cache_size = line_size * cache_associativity * sets_count;
}

std::vector<CacheInfo> CacheInfo::GetAll()
{
    std::vector<CacheInfo> cache_infos;
    unsigned int i = 0;
    while (true)
    {
        CPUID command(CPUID_CACHE_COMMAND, i);
        CacheInfo cache_info(command);

        if (cache_info.type == CacheType::UNDEFINED)
        {
            break;
        }

        cache_infos.push_back(cache_info);

        i++;
    }
    return cache_infos;
}

std::ostream& operator<<(std::ostream& stream, const CacheInfo& cacheInfo)
{
    stream << "cache type                           = " << cacheInfo.type << std::endl;
    stream << "cache level                          = " << cacheInfo.level << std::endl;
    stream << "fully associative cache              = " << cacheInfo.fully_associative << std::endl;
    stream << "system coherency line size           = " << cacheInfo.line_size << std::endl;
    stream << "ways of associativity                = " << cacheInfo.cache_associativity << std::endl;
    stream << "number of sets                       = " << cacheInfo.sets_count << std::endl;
    stream << "total cache size                     = " << cacheInfo.total_cache_size << std::endl;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const CacheType& cacheType)
{
    switch (cacheType)
    {
    case CacheType::UNDEFINED:
        stream << "undefined";
        break;
    case CacheType::DATA:
        stream << "data cache";
        break;
    case CacheType::INSTRUCTION:
        stream << "instruction cache";
        break;
    case CacheType::UNIFIED:
        stream << "unified cache";
        break;
    }
    return stream;
}
