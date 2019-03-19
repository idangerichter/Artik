#include <iostream>
#include "../main/utils/cacheutils.h"
#include "../main/utils/intel.h"

const size_t ITERATIONS = 100;
const size_t LENGTH = L3_CACHE_SIZE / sizeof(uint64_t);
uint64_t secret_array[LENGTH];
uint64_t cache_array[LENGTH];

#define ITERATIONS (100)

int main(void)
{
    long sum_good = 0;
    long sum_bad = 0;
    for (size_t x = 0; x < ITERATIONS; x++)
    {
        // flush array from L3
        for (size_t i = 0; i < LENGTH; i++)
        {
            flush(&secret_array[i]);
            flush(&cache_array[i]);
        }
        // fill L3 with cache array
        for (size_t i = 0; i < LENGTH; i++)
        {
            maccess(&cache_array[i]);
        }

        // secret access
        secret_array[9] = x;

        // try cache attack
        sum_good += probe_timing(&cache_array[9]);
        sum_bad += probe_timing(&cache_array[32]);
    }

    std::cout << "Expected: good > bad" << std::endl;
    std::cout << "good: " << sum_good << std::endl;
    std::cout << "bad: " << sum_bad << std::endl;
    return 0;
}
