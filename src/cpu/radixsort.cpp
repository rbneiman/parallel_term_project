#include "radixsort.h"
#include <algorithm>
#include <iostream>
#include <omp.h>


template<class RandomIterator>
void parallelScan(RandomIterator low, RandomIterator high, RandomIterator outLow, uint32_t mask){
    size_t len = high - low;


    #pragma omp parallel for default(none) firstprivate(low, high, len, outLow, mask)
    for(int blockInd=0; blockInd<16; ++blockInd){
        size_t blockSize = len / 16;
        RandomIterator inBlockLow = low + (blockInd * blockSize);
        RandomIterator outBlockLow = outLow + (blockInd * blockSize);

        //
        size_t sum = 0;
        for(size_t i=0; i<blockSize; ++i){

        }
    }
}


template<class RandomIterator>
void radixSortHelper(RandomIterator low, RandomIterator high, RandomIterator extraLow){

    for(int i=0; i<32; i+=1){
        uint32_t mask = 1 << i;
        parallelScan(low, high, extraLow, mask);
    }
}


namespace cpu_sort{

    void radixSort(std::vector<int32_t> arr, int numThreads){
        omp_set_num_threads(numThreads);

        std::vector<int32_t> extra(arr.size() * 16 , -1);
        radixSortHelper(arr.begin(), arr.end(), extra.begin());
        int j = 0;
    }
}
