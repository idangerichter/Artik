# cache info example
add_executable(CacheInfoExample cache_info_usage.cpp)
target_link_libraries(CacheInfoExample Lib)

# branch detector example
add_executable(BranchDetectorExample L3_fr_branch_detector.cpp)
target_link_libraries(BranchDetectorExample Lib)

# Same array timing detector
add_executable(SameArrayExample L3_fr_same_array_timing.cpp)
target_link_libraries(SameArrayExample Lib)

# Diffrent array timing detector
add_executable(DiffrentArrayExample L3_pp_diff_array_known_location.cpp intel.h)
target_link_libraries(DiffrentArrayExample Lib)

# Random array scan
add_executable(RandomArrayScanExample L3_fr_array_scan_using_random.cpp intel.h)
target_link_libraries(RandomArrayScanExample Lib)