#include "layer_0_cache_intrinsics.hpp"
#include "layer_1_memory_wrapper.hpp"
#include "layer_2_sampler_primitive.hpp"
#include "layer_3_list_sampler.hpp"
#include "layer_3_simple_sampler.hpp"
#include "layer_4_average_sampler.hpp"
#include "layer_5_attack_manager.hpp"
#include "layer_5_attack_manager_without_average.hpp"
#include <iostream>
#include <stdexcept>

constexpr auto SLEEP = 100000; // 0.0001s

std::unique_ptr<Layer>
GetLayer(const std::string& name, MemoryWrapper&& memory_wrapper, size_t first_index, size_t second_index)
{
  if (name == "0")
  {
    return std::make_unique<LayerCacheIntrinsics>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "1")
  {
    return std::make_unique<LayerMemoryWrapper>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "2")
  {
    return std::make_unique<LayerSamplerPrimitive>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "3")
  {
    return std::make_unique<LayerListSampler>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "3s")
  {
    return std::make_unique<LayerSimpleSampler>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "4")
  {
    return std::make_unique<LayerAverageSampler>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "5")
  {
    return std::make_unique<LayerAttackManager>(first_index, second_index, std::move(memory_wrapper));
  }
  else if (name == "5s")
  {
    return std::make_unique<LayerAttackManagerWithoutAverage>(first_index, second_index,
                                                              std::move(memory_wrapper));
  }

  throw std::domain_error("Name is invalid");
}

int main(int argc, char* argv[])
{
  if (argc < 6)
  {
    std::cout << "Usage: layer_tester layer rounds file addr0 addr1" << std::endl;
    return 1;
  }

  std::string layer_name = argv[1];
  size_t rounds = std::strtol(argv[2], nullptr, 10);
  std::string path = argv[3];
  size_t addr0 = std::strtol(argv[4], nullptr, 16);
  size_t addr1 = std::strtol(argv[5], nullptr, 16);

  if (addr0 <= 0 || addr1 <= 0)
  {
    std::cout << "Usage: layer_tester layer file addr0 addr1" << std::endl;
    return 1;
  }

  auto layer = GetLayer(layer_name, MemoryWrapper(path), addr0, addr1);
  for (size_t i = 0; i < rounds; i++)
  {
    auto measurements = layer->Sample();
    std::cout << measurements[0].time << " " << measurements[1].time << std::endl;

    layer->Finalize();

    Layer::Sleep(SLEEP);
  }
  return 0;
}