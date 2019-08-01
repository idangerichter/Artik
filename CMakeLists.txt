cmake_minimum_required(VERSION 2.8)
project(CryptoWorkshop CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_C_COMPILER /usr/bin/gcc)

# static linking - for computer lab compatability
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Cmake output settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Library module
file(GLOB_RECURSE lib_sources src/lib/*.* src/lib/*/.*.)
add_library(Lib ${lib_sources})
set_target_properties(Lib PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(Lib PRIVATE -Wall -Wextra -Wno-attributes -Wno-unused-parameter)


# Playground
add_subdirectory(src/playground)