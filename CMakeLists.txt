cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 52)
endif()

project(parallel_term_project)

set(LANGS C CXX)
message(STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
if(WIN32 OR APPLE)

    if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        set(CUDA_ENABLED "TRUE")
    else()
        set(OPENMP_ENABLED "TRUE")
    endif ()
else()
    set(LANGS C CXX)
#    set(CUDA_ENABLED "TRUE")
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
            src/gpu/bitonicsort.cu
    )
    SET(LANGS ${LANGS} CUDA)
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
    )
endif ()

project(parallel_term_project LANGUAGES ${LANGS})
FIND_PACKAGE( OpenMP)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(json)
include(ips4o)

add_executable(parallel_term_project
    ${SOURCES}
)

target_include_directories(parallel_term_project PRIVATE src)
target_link_libraries(parallel_term_project PRIVATE nlohmann_json::nlohmann_json ips4o)

if(OPENMP_ENABLED)
    FIND_PACKAGE( OpenMP)
    if(OPENMP_FOUND)
        message(STATUS "OpenMP found.")
        target_link_libraries(parallel_term_project PRIVATE OpenMP::OpenMP_CXX)
    else()
        message(FATAL_ERROR, "OpenMP enabled but not found.")
    endif()
endif ()


