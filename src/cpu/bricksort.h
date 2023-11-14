#ifndef PARALLEL_TERM_PROJECT_BRICKSORT_H
#define PARALLEL_TERM_PROJECT_BRICKSORT_H

#include <algorithm>
#include <vector>
#include <omp.h>

namespace cpu_sort {

    template<class RandomIterator>
    void brickSort(RandomIterator begin, RandomIterator end, int numThreads = 4) { // how many threads???
        bool isSorted = false;
        size_t n = std::distance(begin, end);

        omp_set_num_threads(numThreads);

        while (!isSorted) {
            isSorted = true;

            // odd even pass
            #pragma omp parallel for shared(isSorted)
            for (size_t i = 1; i < n - 1; i += 2) {
                if (*(begin + i) > *(begin + i + 1)) {
                    std::iter_swap(begin + i, begin + i + 1);
                    isSorted = false;
                }
            }

            // even odd pass
            #pragma omp parallel for shared(isSorted)
            for (size_t i = 0; i < n - 1; i += 2) {
                if (*(begin + i) > *(begin + i + 1)) {
                    std::iter_swap(begin + i, begin + i + 1);
                    isSorted = false;
                }
            }
        }
    }

    template<typename ValueType>
    void brickSort(std::vector<ValueType>& arr, int numThreads = 4) {
        brickSort(arr.begin(), arr.end(), numThreads);
    }
}

#endif //PARALLEL_TERM_PROJECT_BRICKSORT_H
