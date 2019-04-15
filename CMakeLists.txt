cmake_minimum_required(VERSION 3.10)
project(CryptoWorkshop)

# C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Cmake output settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Library module
file(GLOB_RECURSE lib_sources src/lib/*.* src/lib/*/.*.)
add_library(Lib ${lib_sources} src/lib/timing_agents/timing_agent.hpp src/lib/timing_agents/sleep_timing_agent.hpp src/lib/timing_agents/sleep_timing_agent.cpp src/lib/calibration/calibration.hpp src/lib/calibration/gaussian_mixture_analysis.hpp)
#target_compile_options(Lib PRIVATE -masm=intel)

# Playground
add_subdirectory(src/playground)

