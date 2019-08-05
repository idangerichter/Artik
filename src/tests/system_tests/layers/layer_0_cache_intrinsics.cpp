#include "layer_0_cache_intrinsics.hpp"
#include "../../../lib/low_level/cache_intrinsics.hpp"

LayerCacheIntrinsics::LayerCacheIntrinsics(size_t first_index, size_t second_index, MemoryWrapper&& memory_wrapper) :
  Layer(first_index, second_index, 0, "Cache intrinsics"),
  memory_wrapper_(std::move(memory_wrapper)),
  temp_results_(2)
{
  temp_results_[0].index = first_index;
  temp_results_[1].index = second_index;
}
std::vector<Measurement>& LayerCacheIntrinsics::Sample()
{
  void* ptr1 = &memory_wrapper_[first_index_];
  Memory::MemoryFlush(ptr1);
  Sleep(DELAY);
  auto result1 = Memory::ProbeTiming(ptr1);

  Sleep(DELAY);

  void* ptr2 = &memory_wrapper_[first_index_];
  Memory::MemoryFlush(ptr2);
  Sleep(DELAY);
  auto result2 = Memory::ProbeTiming(ptr2);

  temp_results_[0].time = result1;
  temp_results_[1].time = result2;
  return temp_results_;
}
void LayerCacheIntrinsics::Finalize()
{
  // no-op
}
