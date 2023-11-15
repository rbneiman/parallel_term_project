#ifndef PARALLEL_TERM_PROJECT_QUICKSORT_H
#define PARALLEL_TERM_PROJECT_QUICKSORT_H
#include <algorithm>
#include <vector>
#include <iostream>
#include <tuple>
#include <omp.h>
#include <random>
#include <chrono>

namespace cpu_sort{
    static thread_local std::mt19937_64 prng(std::time(nullptr));

    template<typename RandomIterator>
    RandomIterator quickSortChoosePivot(RandomIterator low, RandomIterator high){
        std::uniform_int_distribution<size_t> dist(0, high - low - 1);

        return low + dist(prng);
    }

    template<typename RandomIterator>
    std::tuple<RandomIterator, RandomIterator> quickSortPartition(RandomIterator pivot, RandomIterator low, RandomIterator high){
        auto p_val = *pivot;
        RandomIterator k = high;

        RandomIterator p = low;
        RandomIterator q = low;
        while(q < k){
//            size_t pivotInd = pivot - low;
//            size_t pInd = p - low;
//            size_t qInd = q - low;
//            size_t kInd = k - low;
            if(*q < p_val){
                std::iter_swap(p,q);
                p += 1;
                q += 1;
            }else if(*q > p_val){
                k -= 1;
                std::iter_swap(q,k);
            }else{
                q += 1;
            }
        }

        return {p,q};
    }

    template<typename RandomIterator>
    void quickSortHelper(RandomIterator low, RandomIterator high){
        if(low >= high){
            return;
        }

        RandomIterator pivot = quickSortChoosePivot(low, high);
        auto [p,q] = quickSortPartition(pivot, low, high);

        #pragma omp task default(none) firstprivate(low, p)
        quickSortHelper(low, p);

        quickSortHelper(q,high);

        #pragma omp taskwait
        {}
    }

    template<typename ValueType>
    void quickSort(std::vector<ValueType>& arr, int numThreads){
        omp_set_num_threads(numThreads);

        #pragma omp parallel sections default(none) shared(arr)
        {
            #pragma omp section
            quickSortHelper(arr.begin(), arr.end());
        }

        int j = 0;
    }
}

#endif //PARALLEL_TERM_PROJECT_QUICKSORT_H
