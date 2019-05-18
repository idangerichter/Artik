#include "../lib/low_level/cache.hpp"
#include <iostream>

int main()
{
    std::vector<CacheInfo> caches = CacheInfo::GetAll();
    int i = 0;
    for (const CacheInfo& cacheInfo : caches)
    {
        std::cout << "Cache " << i << ":" << std::endl;
        std::cout << cacheInfo << std::endl;
        i++;
    }
    return 0;
}