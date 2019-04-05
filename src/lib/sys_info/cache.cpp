#include "cache.hpp"
#include "../utils/bits.hpp"
#include <iostream>
static const unsigned int CPUID_CACHE_COMMAND = 4;

CacheInfo::CacheInfo(const CPUID& c)
: type(static_cast<CacheType>(extractBits(c.EAX(), 0, 4))), level(extractBits(c.EAX(), 5, 7)),
  fully_associative(extractBit(c.EAX(), 9)), line_size(extractBits(c.EBX(), 0, 11) + 1),
  cache_associativity(extractBits(c.EBX(), 22, 31) + 1), sets_count(c.ECX() + 1)
{
    total_cache_size = line_size * cache_associativity * sets_count;
}

std::vector<CacheInfo> CacheInfo::getAll()
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