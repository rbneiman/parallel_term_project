#ifndef PARALLEL_TERM_PROJECT_SORTING_H
#define PARALLEL_TERM_PROJECT_SORTING_H



template<typename ValueType>
bool verify(std::vector<ValueType>& items){
    for(int i=0; i<items.size()-1; ++i){
        if(items[i] > items[i+1]){
            return false;
        }
    }
    return true;
}

#ifdef OPENMP_ENABLED
#include "cpu/bitonicsort.h"
#include "cpu/bricksort.h"
#include "cpu/mergesort.h"
#include "cpu/quicksort.h"
#include "cpu/radixsort.h"
#include <string>
#include <sstream>

template<typename ValueType>
using cpu_sort_fn = void(*)(std::vector<ValueType>&, int);

template<typename ValueType>
void testCPUSorting(std::vector<ValueType>& items, int numThreads){

    std::cout << "std_sort" << '\n';
    {
        std::vector<ValueType> items_copy(items);
        auto t1 = std::chrono::high_resolution_clock::now();
        std::sort(items_copy.begin(), items_copy.end());
        auto t2 = std::chrono::high_resolution_clock::now();
        auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
        std::cout << milis << " ms" << std::endl;
    }

    std::pair<std::string, cpu_sort_fn<ValueType>> sortFuncs[] = {
        {"bitonic_sort", cpu_sort::bitonicSort},
        {"bitonic_sort_recursive", cpu_sort::bitonicSortRecursive},
        {"merge_sort", cpu_sort::mergeSort},
        {"quick_sort", cpu_sort::quickSort}
    };

    for(auto sortFuncPair: sortFuncs){
        std::string& name = sortFuncPair.first;
        auto sortFunc = sortFuncPair.second;
        std::cout << name << '\n';

        std::stringstream times{};
        for(int i = 1; i<=numThreads; ++ i){

            std::vector<ValueType> items_copy(items);
            auto t1 = std::chrono::high_resolution_clock::now();
            sortFunc(items_copy, i);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();

            times << std::to_string(i) ;
            times << ": " << std::to_string(milis) + " ms, ";
            if(!verify(items_copy)){
                std::cerr << "bad sort: " + std::to_string(i) << std::endl;
            }
        }
        std::cout << times.str() << std::endl;



    }

    int j = 0;
}

#endif

#ifdef CUDA_ENABLED
#include "gpu/bitonicsort.cuh"

template<typename ValueType>
void testGPUSorting(std::vector<ValueType>& items){


    std::cout << "bitonic_sort_gpu" << '\n';


    std::vector<ValueType> items_copy(items);
    auto t1 = std::chrono::high_resolution_clock::now();
    gpu_sort::bitonicSort(items_copy);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    std::cout << std::to_string(milis) + " ms, " << std::endl;

    if(!verify(items_copy)){
        std::cerr << "bad sort " << std::endl;
    }
}

#endif


template<typename ValueType>
void testSorting(std::vector<ValueType>& items, int numThreads){

#ifdef OPENMP_ENABLED
    testCPUSorting(items, numThreads);
#endif

#ifdef CUDA_ENABLED
    testGPUSorting(items);
#endif

}





#endif //PARALLEL_TERM_PROJECT_SORTING_H
