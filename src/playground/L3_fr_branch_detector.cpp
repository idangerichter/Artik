#include "../lib/utils/Board.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const size_t LIMIT = 120;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: branch_detector file addr0 addr1" << std::endl;
        return 1;
    }

    std::string path(argv[1]);
    int addr0 = std::strtol(argv[2], nullptr, 16);
    int addr1 = std::strtol(argv[3], nullptr, 16);

    if (addr0 <= 0 || addr1 <= 0)
    {
        std::cout << "Usage: branch_detector file addr0 addr1" << std::endl;
        return 1;
    }
    std::cout << "Detecting between: " << addr0 << " and " << addr1 << std::endl;
    Board board(path);

    uint total0 = 0;
    uint total1 = 0;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true)
    {
        board.Flush(addr0);
        usleep(50);
        int32_t result0 = board.Measure(addr0);
        usleep(50);
        board.Flush(addr1);
        usleep(50);
        int32_t result1 = board.Measure(addr1);
        if (result0 <= LIMIT)
        {
            total0++;
            std::cout << "Case 0 triggered with " << result0 << " cycles. Total: 0s: " << total0
                      << " 1s: " << total1 << std::endl;
        }
        if (result1 <= LIMIT)
        {
            total1++;
            std::cout << "Case 1 triggered with " << result1 << " cycles. Total: 0s: " << total0
                      << " 1s: " << total1 << std::endl;
        }
    }
#pragma clang diagnostic pop
}
