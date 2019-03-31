#pragma once
#include "cpu_info.h"

size_t get_threshold_L1(const cpu_info& info);
size_t get_threshold_L2(const cpu_info& info);
size_t get_threshold_L3(const cpu_info& info);
