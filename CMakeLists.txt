cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 52)
endif()

project(parallel_term_project LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(parallel_term_project
    src/main.cpp
    src/gpu/algo_test.cu
    src/testcase.cpp
)

