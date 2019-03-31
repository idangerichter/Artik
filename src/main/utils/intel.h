#ifndef INTEL_H
#define INTEL_H

/**
 * The following information is correct for i7-4770 (yoav's PC).
 * It is based on the following source: https://www.7-cpu.com/cpu/Haswell.html
 */

#pragma region L1
const size_t L1_DATA_CACHE_SIZE = 32 * 1024; // 32KB
const size_t L1_INSTRUCTIONS_CACHE_SIZE = 32 * 1024; // 32KB
const size_t L1_LINE_SIZE = 64; // 64B
const size_t L1_N_WAY = 8;
#pragma endregion

#pragma region L2
const size_t L2_CACHE_SIZE = 256 * 1024; // 256KB
const size_t L2_LINE_SIZE = 64; // 64B
const size_t L2_N_WAY = 8;
#pragma endregion

#pragma region L3
const size_t L3_CACHE_SIZE = 8 * 1024 * 1024; // 8MB
const size_t L3_LINE_SIZE = 64; // 64B
#pragma endregion

#endif
