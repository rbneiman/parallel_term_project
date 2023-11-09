#include "distributions.h"
#include <iostream>
#include "cpu/mergesort.h"

using value_t = double;
using dist_fptr = distributions::dist_gen_fptr_t<value_t>;

int main(){
    dist_fptr distributions[] = {
        distributions::shuffled_random(),
        distributions::shuffled_similar(),
        distributions::all_equal(),
        distributions::ascending(),
        distributions::descending(),
        distributions::swapped_endpoints()
    };
    size_t sizes[] = {
            4, 8, 16, 64, 128
    };

    for(size_t size : sizes){
        std::cout << "Size: " << size << '\n';
        for(dist_fptr dist: distributions){
            std::vector<value_t> items = dist(size);
            std::vector<value_t> items_copy(items);

            cpu_sort::mergeSort(items_copy, 1);
            std::cout << std::flush;
            int j = 0;
        }
    }


    return 0;
}