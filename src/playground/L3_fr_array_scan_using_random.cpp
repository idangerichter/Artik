#include "../lib/low_level/cache_intrinsics.h"
#include "intel.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

const size_t INNER_ITERATIONS = 64 * 11; // need 2 * nlogn, and n = 64;
const size_t LIMIT = 300;
const size_t LENGTH = L3_CACHE_SIZE / sizeof(uint64_t);
uint64_t secret_array[LENGTH];

/**
 * Copied code from stackoverflow, does whatever you think it does.
 */
void print_bits(size_t const size, void const* const ptr)
{
    unsigned char* b = (unsigned char*)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
    {
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void child_process(uint64_t array[])
{
    std::cout << "Child process running" << std::endl;
    while (true)
    {
        maccess(&array[4 * 8]);
        maccess(&array[25 * 8]);
    }
}

void parent_process(uint64_t array[])
{
    std::cout << "Parent process running" << std::endl;
    while (true)
    {
        int64_t sum = 0;

        for (auto y = 0; y < INNER_ITERATIONS; y++)
        {
            // flush the array out
            for (auto i = 0; i < 64; i++)
            {
                flush(&array[i * 8]);
            }
            // wait
            usleep(15 * 100);
            // check if random element has returned
            int r = rand() % 64;
            sum = sum | ((int64_t)(probe_timing(&array[r * 8]) < LIMIT) << r);
        }

        print_bits(sizeof(sum), &sum);
    }
}

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(time(0)));

    // allocate
    auto array = new uint64_t[LENGTH];

    if (fork())
    {
        child_process(array);
    }
    else
    {
        parent_process(array);
    }

    return 0;
}