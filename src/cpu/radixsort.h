#ifndef PARALLEL_TERM_PROJECT_RADIXSORT_H
#define PARALLEL_TERM_PROJECT_RADIXSORT_H
#include <cstdint>
#include <vector>

namespace cpu_sort{


    void radixSort(std::vector<int32_t> arr, int numThreads);

}

#endif //PARALLEL_TERM_PROJECT_RADIXSORT_H
