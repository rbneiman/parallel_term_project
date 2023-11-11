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

template<typename ValueType>
void testCPUSorting(std::vector<ValueType>& items, int numThreads){
    std::vector<ValueType> items_copy(items);

    cpu_sort::mergeSort(items_copy, numThreads);

    if(!verify(items_copy)){
        std::cerr << "bad sort" << std::endl;
    }
    int j = 0;
}

#endif

#ifdef CUDA_ENABLED
#include "gpu/bitonicsort.h"
#include "gpu/bricksort.h"
#include "gpu/mergesort.h"
#include "gpu/quicksort.h"
#include "gpu/radixsort.h"

template<typename ValueType>
void testGPUSorting(std::vector<ValueType>& arr){

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
