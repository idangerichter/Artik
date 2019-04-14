#include "../lib/low_level/cache_intrinsics.hpp"
#include "../lib/utils/attack_surface.hpp"
#include "intel.h"
#include <iostream>

const size_t ITERATIONS = 100;
const size_t LENGTH = L3_CACHE_SIZE / sizeof(uint64_t);
uint64_t secret_array[LENGTH];


#define ITERATIONS (100)

int main()
{
    AttackSurface board(LENGTH);
    long sum_good = 0;
    long sum_bad = 0;
    for (size_t x = 0; x < ITERATIONS; x++)
    {
        // fill L3 with cache array
        for (size_t i = 0; i < LENGTH; i++)
        {
            board.Access(i);
        }

        // secret access
        secret_array[9] = x;

        // try cache attack
        sum_good += board.Measure(9);
        sum_bad += board.Measure(47);
    }

    std::cout << "Expected: good > bad" << std::endl;
    std::cout << "good: " << sum_good << std::endl;
    std::cout << "bad: " << sum_bad << std::endl;
    return 0;
}
