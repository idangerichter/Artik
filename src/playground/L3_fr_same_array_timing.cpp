#include "../lib/low_level/cache_intrinsics.hpp"
#include "../lib/utils/attack_surface.hpp"
#include "intel.h"
#include <iostream>

const size_t ITERATIONS = 100;
const size_t LENGTH = L3_CACHE_SIZE / sizeof(uint64_t);

int main()
{
    AttackSurface secret_array(LENGTH);
    long sum_good = 0;
    long sum_bad = 0;
    for (size_t x = 0; x < ITERATIONS; x++)
    {
        // flush array from L3
        for (size_t i = 0; i < LENGTH; i++)
        {
            secret_array.Flush(i);
        }

        // secret access
        secret_array[9] = x;

        // try cache attack
        sum_good += secret_array.Measure(9);
        sum_bad += secret_array.Measure(47);
    }

    std::cout << "Expected: good < bad" << std::endl;
    std::cout << "good: " << sum_good << std::endl;
    std::cout << "bad: " << sum_bad << std::endl;
    return 0;
}