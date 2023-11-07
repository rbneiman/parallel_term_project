cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 52)
endif()

project(parallel_term_project LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(parallel_term_project
    src/main.cpp
    src/testcase.cpp

    src/cpu/bitonicsort.cpp
    src/cpu/bricksort.cpp
    src/cpu/mergesort.cpp
    src/cpu/quicksort.cpp
    src/cpu/radixsort.cpp

    src/gpu/bitonicsort.cpp
    src/gpu/bricksort.cpp
    src/gpu/mergesort.cpp
    src/gpu/quicksort.cpp
    src/gpu/radixsort.cpp
)

