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
                       RandomIterator outLow, RandomIterator outHigh, RandomIterator orig){
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

        if(diff1 < 511){
            std::merge(first1, last1, first2, last2, outLow);
            return;
        }
        size_t midInd = ((last1 - first1)/2);
        RandomIterator mid1 = first1 + ((last1 - first1)/2);
        RandomIterator search2 = std::lower_bound(first2, last2, *mid1);
        RandomIterator outInd = outLow + (mid1 - first1) + (search2 - first2);
        size_t midOff = mid1 - first1;
        size_t ind = search2 - first2;
//        std::cout << "M Out: " + std::to_string(outLow - orig) + " - " + std::to_string(outHigh - orig - 1)
//                     + " ind:" + std::to_string(outInd - orig)
//                     + " val: " + std::to_string(*mid1) + "\n" << std::endl;
        *outInd = *mid1;


        #pragma omp task default(none) firstprivate(first1, mid1, first2, search2, outLow, outInd, orig)
        {
            parallelMerge(first1, mid1, first2, search2, outLow, outInd, orig);
        }


        parallelMerge(mid1+1, last1, search2, last2, outInd+1, outHigh, orig);

        #pragma omp taskwait
        {
            ;
        };
    }

    template<class RandomIterator>
    void mergeSortHelper(RandomIterator low, RandomIterator high, RandomIterator outLow, RandomIterator outHigh,
                         RandomIterator outOrig, RandomIterator orig, int layer, int layerSize){
//        std::cout << "D Layer: " + std::to_string(layer) + " Ind: " + std::to_string(low - orig)
//              + " - " + std::to_string(high - orig) << std::endl;


        size_t inSize = high - low;
        size_t outLowInd = outLow - outOrig;
        size_t outHighInd = outHigh - outOrig;
        if(high < low){
            return;
        }else if(high == low){
//            std::cout << "L Out size: " + std::to_string(outHigh - outLow + 1)
//                         + " ind:" + std::to_string(outLow - outOrig)
//                         + " val: " + std::to_string(*low) + "\n";
//            std::cout << std::endl;
            *outLow = *low;
            return;
        }
//
//        if(inSize<512){
//            size_t arrSize = high - low;
//            size_t outSize = outHigh - outLow;
//            int j=0;
//            auto out = std::copy(low, high+1, outLow);
//            size_t copied = out - outLow;
//            std::sort(outLow, outHigh);
//            return;
//        }

        size_t diff = (high - low) / 2;
        size_t outOffset = (high - low) + 1;
        RandomIterator mid = low + diff;

        RandomIterator outLowOffset =outLow+layerSize;
        RandomIterator outMidOffset = outLow+diff+layerSize;
        RandomIterator outHighOffset = outHigh+layerSize;


        #pragma omp task default(none) firstprivate(low, mid, outLowOffset, outMidOffset, outOrig, orig, layer, outOffset, layerSize)
        {
            mergeSortHelper(low, mid, outLowOffset, outMidOffset, outOrig, orig, layer + 1, layerSize);
        }

        mergeSortHelper(mid + 1, high, outMidOffset+1, outHighOffset, outOrig, orig, layer+1, layerSize);

        #pragma omp taskwait
        parallelMerge(outLowOffset, outMidOffset+1, outMidOffset+1, outHighOffset+1, outLow, outHigh + 1, outOrig);
    }

    static size_t test(size_t n){
        return (std::log2(n) + 1) * n;
    }

    template<typename ValueType>
    void mergeSort(std::vector<ValueType>& arr, int numThreads){
        omp_set_num_threads(numThreads);

        test(1);
        size_t size = (std::log2(arr.size()) + 1) * arr.size();
        std::vector<ValueType> out(size , -1);
        #pragma omp parallel default(none) shared(arr, out)
        {
            mergeSortHelper(arr.begin(), arr.end() - 1,
                            out.begin(), out.begin() + arr.size() - 1,
                            out.begin(), arr.begin(), 0, arr.size());
        };
        out.resize(arr.size());
        arr = std::move(out);
    }

}

#endif //PARALLEL_TERM_PROJECT_MERGESORT_H
