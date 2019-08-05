cmake_minimum_required(VERSION 3.5)

# see https://public.kitware.com/pipermail/cmake/2009-November/033133.html
if (EXISTS /usr/local/bin/g++-8.3.0)
    set(CMAKE_CXX_COMPILER /usr/local/bin/g++-8.3.0)
    set(CMAKE_C_COMPILER /usr/local/bin/gcc++-8.3.0)
else ()
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
    set(CMAKE_C_COMPILER /usr/bin/gcc)
endif ()

project(CryptoWorkshop CXX)

# Cmake settings
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# C++ settings
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# Testing
include(cmake/googletest.cmake)
fetch_googletest(
        ${PROJECT_SOURCE_DIR}/cmake
        ${PROJECT_BINARY_DIR}/googletest
)
enable_testing()
add_subdirectory(src/tests)

# Playground
add_subdirectory(src/playground)