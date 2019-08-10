#include "cache.hpp"
#include "../utils/bits.hpp"
#include <iostream>

/**
 * We use this scheme for extraction
 *
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

namespace
{
CacheType GetType(const CPUID& cid)
{
  return static_cast<CacheType>(ExtractBits(cid.eax, 0, 4));
}

int32_t GetLevel(const CPUID& cid)
{
  return ExtractBits(cid.eax, 5, 7);
}
bool GetFullyAssociative(const CPUID& cid)
{
  return ExtractBit(cid.eax, 9);
}

int32_t GetLineSize(const CPUID& cid)
{
  return ExtractBits(cid.ebx, 0, 11) + 1;
}

int32_t GetCacheAssociativity(const CPUID& cid)
{
  return ExtractBits(cid.ebx, 22, 31) + 1;
}

int32_t GetSetsCount(const CPUID& cid)
{
  return cid.ecx + 1;
}
} // namespace

CacheInfo::CacheInfo(const CPUID& c) :
  type_(GetType(c)),
  level_(GetLevel(c)),
  fully_associative_(GetFullyAssociative(c)),
  line_size_(GetLineSize(c)),
  cache_associativity_(GetCacheAssociativity(c)),
  sets_count_(GetSetsCount(c)),
  total_cache_size_(line_size_ * cache_associativity_ * sets_count_)
{
}

std::vector<CacheInfo> CacheInfo::GetAll()
{
  std::vector<CacheInfo> cache_infos;
  for (size_t i = 0;; ++i)
  {
    CPUID command(CPUID_CACHE_COMMAND, i);
    CacheInfo cache_info(command);

    if (cache_info.type_ == CacheType::UNDEFINED)
    {
      break;
    }

    cache_infos.push_back(cache_info);
  }
  return cache_infos;
}

std::ostream& operator<<(std::ostream& stream, const CacheInfo& cacheInfo)
{
  stream << "cache type                           = " << cacheInfo.type_ << std::endl;
  stream << "cache level                          = " << cacheInfo.level_ << std::endl;
  stream << "fully associative cache              = " << cacheInfo.fully_associative_
         << std::endl;
  stream << "system coherency line size           = " << cacheInfo.line_size_ << std::endl;
  stream << "ways of associativity                = " << cacheInfo.cache_associativity_
         << std::endl;
  stream << "number of sets                       = " << cacheInfo.sets_count_ << std::endl;
  stream << "total cache size                     = " << cacheInfo.total_cache_size_
         << std::endl;
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
