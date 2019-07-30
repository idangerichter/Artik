cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_C_COMPILER /usr/bin/gcc)
project(CryptoWorkshop CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Cmake output settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Library module
file(GLOB_RECURSE lib_sources src/lib/*.* src/lib/*/.*.)
add_library(Lib ${lib_sources})
set_target_properties(Lib PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(Lib PRIVATE -Wall -Wextra -Wno-attributes)


# Playground
add_subdirectory(src/playground)