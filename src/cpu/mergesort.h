#ifndef PARALLEL_TERM_PROJECT_MERGESORT_H
#define PARALLEL_TERM_PROJECT_MERGESORT_H
#include <algorithm>
#include <vector>
#include <atomic>
#include <iostream>
#include <string>

#include "ThreadPool.h"

namespace cpu_sort{

    template<typename ValueType>
    void mergeSort(std::vector<ValueType>& arr, unsigned numThreads){
        ThreadPool threadPool{};
        threadPool.start(numThreads);

        std::vector<ValueType> out(arr.size());
        auto fut = threadPool.queueTask([&arr, &out, &threadPool]{
            mergeSortHelper(arr.begin(), arr.end()-1, out.begin(), out.end()-1, out.begin(), threadPool);
        });
        threadPool.waitUntilDone();


        threadPool.stop();
        arr = std::move(out);
    }

    template<class RandomIterator>
    void mergeSortHelper(RandomIterator low, RandomIterator high, RandomIterator outLow, RandomIterator outHigh,
                         RandomIterator orig,
                         ThreadPool& threadPool){
        bool res = high < low;
        size_t t = outLow - orig;
        size_t t2 = outHigh - orig;
        if(high < low){
            return;
        }else if(high == low){
            std::cout << "Out size: " + std::to_string(outHigh - outLow)
                         + " ind:" + std::to_string(outLow - orig)
                         + " val: " + std::to_string(*low) + "\n";
            *outLow = *low;
            return;
        }

        size_t diff = (high - low) / 2;
        RandomIterator mid = low + diff;
        RandomIterator outMid = outLow + diff;

        std::shared_ptr<std::atomic_int> onDone(new std::atomic_int(0));
        threadPool.queueTask([low, high, mid, outLow, outHigh, outMid, &threadPool, onDone, orig]{
            mergeSortHelper(low, mid, outLow, outMid, orig, threadPool);
            int val = onDone->fetch_add(1);
            if(val == 1){
                parallelMerge(low, mid, mid+1, high, outLow, outHigh,  orig,threadPool);
            }
        });


        mergeSortHelper(mid + 1, high, outMid + 1, outHigh,  orig, threadPool);
        int val = onDone->fetch_add(1);
        if(val == 1){
            parallelMerge(low, mid, mid+1, high, outLow, outHigh, orig, threadPool);
        }
    }

    template<class RandomIterator>
    void parallelMerge(RandomIterator first1, RandomIterator last1,
                       RandomIterator first2, RandomIterator last2,
                       RandomIterator outLow, RandomIterator outHigh, RandomIterator orig, ThreadPool& threadPool){

        size_t diff1 = last1 - first1;
        size_t diff2 = last2 - first2;
        if(diff2 > diff1){
            std::swap(first1, first2);
            std::swap(last1, last2);
            std::swap(diff1, diff2);
        }

        if(diff1 <= 0){
            return;
        }

        RandomIterator mid = first1 + ((last1 - first1)/2);
        RandomIterator searchInd = std::lower_bound(first2, last2, *mid);
        RandomIterator outInd = outLow + (mid - first1) + (searchInd - first2);
        std::cout << "Out size: " + std::to_string(outHigh - outLow)
        + " ind:" + std::to_string(outInd - orig)
        + " val: " + std::to_string(*mid) + "\n";
        *outInd = *mid;


        auto fut = threadPool.queueTask([first1, mid, first2, searchInd, outLow, outInd, &threadPool, orig]{
            parallelMerge(first1, mid, first2, searchInd, outLow, outInd,  orig,threadPool);
        });

        parallelMerge(mid+1, last1, searchInd, last2, outInd+1, outHigh, orig, threadPool);
    }
}

#endif //PARALLEL_TERM_PROJECT_MERGESORT_H
