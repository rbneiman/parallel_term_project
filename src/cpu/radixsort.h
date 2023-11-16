#ifndef PARALLEL_TERM_PROJECT_RADIXSORT_H
#define PARALLEL_TERM_PROJECT_RADIXSORT_H

#include <algorithm>
#include <iostream>
#include <omp.h>

namespace cpu_sort {

    template<typename ValueType>
    ValueType getMax(ValueType array[], int n) {
        ValueType max = array[0];
        #pragma omp parallel for reduction(max:max)
        for (int i = 1; i < n; i++) {
            if (array[i] > max)
                max = array[i];
        }
        return max;
    }

    template<typename ValueType>
    void countingSort(ValueType array[], int size, int place) {
        const int max = 10;
        ValueType output[size];
        int count[max] = {0};

        // Calculate count of elements
        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            #pragma omp atomic
            count[(array[i] / place) % 10]++;
        }

        // Calculate cumulative count
        for (int i = 1; i < max; i++)
            count[i] += count[i - 1];

        // Place the elements in sorted order
        for (int i = size - 1; i >= 0; i--) {
            output[count[(array[i] / place) % 10] - 1] = array[i];
            count[(array[i] / place) % 10]--;
        }

        // Copy the output array back to the original array
        #pragma omp parallel for
        for (int i = 0; i < size; i++)
            array[i] = output[i];
    }

    template<typename ValueType>
    void radixsort(ValueType array[], int size) {
        // Get maximum element
        ValueType max = getMax(array, size);

        // Apply counting sort to sort elements based on place value.
        for (int place = 1; max / place > 0; place *= 10)
            countingSort(array, size, place);
    }

} 

#endif //PARALLEL_TERM_PROJECT_RADIXSORT_H
