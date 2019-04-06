#include "../lib/utils/Board.hpp"
#include "intel.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

#define INNER_ITERATIONS (64 * 11) // need 2 * nlogn, and n = 64;
#define LIMIT 300
#define LENGTH L3_CACHE_SIZE

/**
 * Copied code from stackoverflow, does whatever you think it does.
 */
void print_bits(size_t const size, void const* const ptr)
{
    auto* b = (unsigned char*)ptr;
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

void child_process(Board& board)
{
    std::cout << "Child process running" << std::endl;
    while (true)
    {
        board.Access(4 * 8);
        board.Access(25 * 8);
    }
}


void parent_process(Board& board)
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
                board.Flush(i * 8);
            }
            // wait
            usleep(15 * 100);
            // check if random element has returned
            int r = rand() % 64;
            sum = sum | ((int64_t)(board.Measure(r * 8) < LIMIT) << r);
        }

        print_bits(sizeof(sum), &sum);
    }
}

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // allocate
    Board board(LENGTH);

    if (fork())
    {
        child_process(board);
    }
    else
    {
        parent_process(board);
    }

    return 0;
}
