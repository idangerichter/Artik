cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
project(CryptoWorkshop CXX)

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
add_library(Lib ${lib_sources})
set_target_properties(Lib PROPERTIES LINKER_LANGUAGE CXX)


# Playground
add_subdirectory(src/playground)