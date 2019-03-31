#pragma once
#include <cstdint>

struct cpu_info
{
  #pragma region L1
  size_t L1_data_cache_size;
  size_t L1_instructions_cache_size;
  size_t L1_line_size;
  size_t L1_n_way;
  #pragma endregion

  #pragma region L2
  size_t L2_cache_size;
  size_t L2_line_size; 
  size_t L2_n_way;
  #pragma endregion

  #pragma region L3
  size_t L3_cache_size; 
  size_t L3_line_size; 
  #pragma endregion

  // TODO: call system to get all this info
};