#ifndef PARALLEL_TERM_PROJECT_BITONICSORT_H
#define PARALLEL_TERM_PROJECT_BITONICSORT_H
#include <algorithm>
#include <vector>
#include <iostream>
#include <omp.h>

namespace cpu_sort{

    template<class RandomIterator>
    void parallelBitonicMerge(RandomIterator low, RandomIterator high, bool isAscending){
        const size_t len = high - low;
        const size_t m = len / 2;


        if(len > 1){
            const size_t loopBound = (low + m) - low;
            #pragma omp parallel loop default(none) firstprivate(low, isAscending, m, len, loopBound)
            for(size_t i=0; i<loopBound; ++i){
                RandomIterator val1 = low + i;
                RandomIterator val2 = low + i + m;
                if((isAscending && (*val1 > *val2))
                    || (!isAscending && (*val1 < *val2))){
                    std::iter_swap(val1, val2);
                }
            }

            #pragma omp task default(none) firstprivate(low, m, isAscending)
            {
                parallelBitonicMerge(low, low + m, isAscending);
            }
            parallelBitonicMerge(low + m, high, isAscending);

            #pragma omp taskwait
            {};
        }
        int j= 0;
    }

    template<class RandomIterator>
    void bitonicSortHelper(RandomIterator low, RandomIterator high,  bool isAscending){
        size_t len = high - low;

        if(len<512){
            if(isAscending){
                std::sort(low, high);
            }else{
                std::sort(low, high, std::greater<>{});
            }
            return;
        }

        if(len > 1){
            size_t m = len / 2;

            #pragma omp task default(none) firstprivate(low, m)
            {
                bitonicSortHelper(low, low + m, true);
            }
            bitonicSortHelper(low + m, high, false);

            #pragma omp taskwait
            parallelBitonicMerge(low, high, isAscending);
        }
    }

    template<typename ValueType>
    void bitonicSort(std::vector<ValueType>& arr, int numThreads){
        omp_set_num_threads(numThreads);

        #pragma omp parallel sections default(none) shared(arr)
        {
            #pragma omp section
            bitonicSortHelper(arr.begin(), arr.end(), true);
        }
        int j = 0;
    }
}

#endif //PARALLEL_TERM_PROJECT_BITONICSORT_H
