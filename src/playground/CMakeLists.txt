# cache info example
add_executable(CacheInfoExample cache_info_usage.cpp)
target_link_libraries(CacheInfoExample Lib)

# branch detector example
add_executable(BranchDetectorExample L3_fr_branch_detector.cpp)
target_link_libraries(BranchDetectorExample Lib)

# The attack
add_executable(HostapdAttacker hostapd_attacker.cpp)
target_link_libraries(HostapdAttacker Lib)

