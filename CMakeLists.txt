cmake_minimum_required(VERSION 3.21)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 52)
endif()


if(WIN32)
    if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        set(LANGS C CXX CUDA)
        set(CUDA_ENABLED "TRUE")
    else()
        set(LANGS C CXX)
        set(OPENMP_ENABLED "TRUE")
    endif ()
else()
    set(LANGS C CXX CUDA)
    set(CUDA_ENABLED "TRUE")
    set(OPENMP_ENABLED "TRUE")
endif ()

set(SOURCES

    src/main.cpp
    src/testcase.cpp
)

if(CUDA_ENABLED)
    message(STATUS "CUDA Enabled.")
    add_compile_definitions(CUDA_ENABLED)
    SET(SOURCES ${SOURCES}
            src/gpu/bitonicsort.cpp
            src/gpu/bricksort.cpp
            src/gpu/mergesort.cpp
            src/gpu/quicksort.cpp
            src/gpu/radixsort.cpp
    )
endif ()

if(OPENMP_ENABLED)
    message(STATUS "OpenMP Enabled.")
    add_compile_definitions(OPENMP_ENABLED)
    SET(SOURCES ${SOURCES}
            src/cpu/bitonicsort.cpp
            src/cpu/bricksort.cpp
            src/cpu/mergesort.cpp
            src/cpu/quicksort.cpp
            src/cpu/radixsort.cpp
            src/cpu/threadPool.cpp
    )
endif ()

project(parallel_term_project LANGUAGES ${LANGS})
FIND_PACKAGE( OpenMP)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(parallel_term_project
    ${SOURCES}
)

if(OPENMP_ENABLED)
    FIND_PACKAGE( OpenMP)
    if(OPENMP_FOUND)
        message(STATUS "OpenMP found.")
        target_link_libraries(parallel_term_project PUBLIC OpenMP::OpenMP_CXX)
    else()
        message(FATAL_ERROR, "OpenMP enabled but not found.")
    endif()
endif ()


