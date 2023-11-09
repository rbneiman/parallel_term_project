cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 52)
endif()

project(parallel_term_project LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

#find openMP and add relevent flags to compiler
FIND_PACKAGE( OpenMP)
if(OPENMP_FOUND)
    message(STATUS "OpenMP found, adding flags")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
include_directories(SYSTEM ${OpenMP_INCLUDE_PATH})

add_executable(parallel_term_project
    src/main.cpp
    src/testcase.cpp

    src/cpu/bitonicsort.cpp
    src/cpu/bricksort.cpp
    src/cpu/mergesort.cpp
    src/cpu/quicksort.cpp
    src/cpu/radixsort.cpp
    src/cpu/threadPool.cpp

    src/gpu/bitonicsort.cpp
    src/gpu/bricksort.cpp
    src/gpu/mergesort.cpp
    src/gpu/quicksort.cpp
    src/gpu/radixsort.cpp
)

