#ifndef PARALLEL_TERM_PROJECT_MERGESORT_H
#define PARALLEL_TERM_PROJECT_MERGESORT_H
#include <algorithm>
#include <vector>
#include <atomic>
#include <iostream>
#include <string>
#include <cmath>
#include <omp.h>


namespace cpu_sort{

    template<class RandomIterator>
    void parallelMerge(RandomIterator first1, RandomIterator last1,
                       RandomIterator first2, RandomIterator last2,
                       RandomIterator outLow, RandomIterator outHigh){
        ssize_t diff1 = last1 - first1;
        ssize_t diff2 = last2 - first2;
        if(diff2 > diff1){
            std::swap(first1, first2);
            std::swap(last1, last2);
            std::swap(diff1, diff2);
        }

        if(diff1 <= 0){
            return;
        }

        if(diff1 < 512){
            std::merge(first1, last1, first2, last2, outLow);
            return;
        }
        RandomIterator mid1 = first1 + ((last1 - first1)/2);
        RandomIterator search2 = std::lower_bound(first2, last2, *mid1);
        RandomIterator outInd = outLow + (mid1 - first1) + (search2 - first2);
        *outInd = *mid1;


        #pragma omp task default(none) firstprivate(first1, mid1, first2, search2, outLow, outInd)
        {
            parallelMerge(first1, mid1, first2, search2, outLow, outInd);
        }


        parallelMerge(mid1+1, last1, search2, last2, outInd+1, outHigh);

        #pragma omp taskwait
        {
            ;
        };
    }

    template<class RandomIterator>
    void mergeSortHelper(RandomIterator low, RandomIterator high, RandomIterator outLow, RandomIterator outHigh, size_t layerSize){

        size_t inSize = high - low;
        if(high < low){
            return;
        }else if(high == low){
            *outLow = *low;
            return;
        }

        if(inSize<512){
            std::copy(low, high+1, outLow);
            std::sort(outLow, outHigh + 1);
            return;
        }

        size_t diff = (high - low) / 2;
        size_t outOffset = (high - low) + 1;
        RandomIterator mid = low + diff;

        RandomIterator outLowOffset =outLow+layerSize;
        RandomIterator outMidOffset = outLow+diff+layerSize;
        RandomIterator outHighOffset = outHigh+layerSize;

        #pragma omp task default(none) firstprivate(low, mid, outLowOffset, outMidOffset, outOffset, layerSize)
        {
            mergeSortHelper(low, mid, outLowOffset, outMidOffset, layerSize);
        }

        #pragma omp task default(none) firstprivate(mid, high, outMidOffset, outHighOffset, outOffset, layerSize)
        mergeSortHelper(mid + 1, high, outMidOffset+1, outHighOffset, layerSize);

        #pragma omp taskwait
        parallelMerge(outLowOffset, outMidOffset+1, outMidOffset+1, outHighOffset+1, outLow, outHigh + 1);
    }
//
//    template<class RandomIterator>
//    void mergeSortTest(RandomIterator low, RandomIterator high, RandomIterator outLow, RandomIterator outHigh){
//
//        size_t diff = (high - low) / 2;
//        RandomIterator mid = low + diff;
//        #pragma omp task default(none)  firstprivate(low, mid)
//        std::sort(low, mid);
//
//        #pragma omp task default(none)  firstprivate(low, mid)
//        std::sort(mid+1, high);
//
//        #pragma omp taskwait
//        std::merge(low, mid, mid+1, high, outLow,)
//    }

    template<typename ValueType>
    void mergeSort(std::vector<ValueType>& arr, int numThreads){
        omp_set_num_threads(numThreads);

        size_t size = (std::log2(arr.size()) + 1) * arr.size();
        std::vector<ValueType> out(size , -1);
        #pragma omp parallel sections default(none) shared(arr, out)
        {
            #pragma omp section
            mergeSortHelper(arr.begin(), arr.end() - 1,
                            out.begin(), out.begin() + arr.size() - 1,
                            arr.size());
        }
        out.resize(arr.size());
        arr = std::move(out);
    }

}

#endif //PARALLEL_TERM_PROJECT_MERGESORT_H
