#ifndef PARALLEL_TERM_PROJECT_RADIXSORT_H
#define PARALLEL_TERM_PROJECT_RADIXSORT_H

#include <algorithm>
#include <vector>
#include <omp.h>

namespace cpu_sort {

    template<typename ValueType>
    void parallelRadixSort(std::vector<ValueType>& arr) {
        const int numDigits = 10; // For decimal system
        int maxVal = *std::max_element(arr.begin(), arr.end());

        std::vector<std::vector<int>> count(omp_get_max_threads(), std::vector<int>(numDigits));
        std::vector<ValueType> temp(arr.size());

        for (int exp = 1; maxVal/exp > 0; exp *= 10) {
            for (auto &c : count) {
                std::fill(c.begin(), c.end(), 0);
            }

            // Parallel Counting
            #pragma omp parallel
            {
                int threadNum = omp_get_thread_num();
                #pragma omp for
                for (size_t i = 0; i < arr.size(); i++) {
                    int digit = (arr[i] / exp) % numDigits;
                    count[threadNum][digit]++;
                }
            }

            // parallelize prefix sum calc 
            for (int digit = 0; digit < numDigits; digit++) {
                int sum = 0;
                #pragma omp parallel for reduction(+:sum)
                for (int t = 0; t < omp_get_max_threads(); t++) {
                    int tmp = count[t][digit];
                    count[t][digit] = sum;
                    sum += tmp;
                }
            }

            // parallel reordering???
            #pragma omp parallel for
            for (int t = 0; t < omp_get_max_threads(); t++) {
                for (size_t i = 0; i < arr.size(); i++) {
                    if (i/omp_get_max_threads() == t) {
                        int digit = (arr[i] / exp) % numDigits;
                        temp[count[t][digit]++] = arr[i];
                    }
                }
            }

            arr = temp;
        }
    }
}

#endif //PARALLEL_TERM_PROJECT_RADIXSORT_H
