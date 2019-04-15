#include "../lib/sys_info/cache.hpp"
#include <iostream>

int main()
{
    std::vector<CacheInfo> caches = CacheInfo::GetAll();
    int i = 0;
    for (CacheInfo cacheInfo : caches)
    {
        std::cout << "Cache " << i << ":" << std::endl;
        std::cout << cacheInfo << std::endl;
        i++;
    }
    return 0;
}