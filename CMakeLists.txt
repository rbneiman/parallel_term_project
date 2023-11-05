cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 52)
endif()

project(parallel_term_project LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

