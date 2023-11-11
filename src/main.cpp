#include "distributions.h"
#include "sorting.h"
#include <iostream>


using value_t = double;
using dist_fptr = distributions::dist_gen_fptr_t<value_t>;

int main(){
    dist_fptr distributions[] = {
        distributions::shuffled_ascending(),
        distributions::shuffled_similar(),
        distributions::all_equal(),
        distributions::ascending(),
        distributions::descending(),
        distributions::swapped_endpoints()
    };
    size_t sizes[] = {
            4, 8, 16, 64, 128
    };

    distributions::seed_prng(0);

    size_t arrSize = 1 << 14;
    for(int i=0; i<1; ++i){
        std::cout << "Size: " << arrSize << '\n';
        auto t1 = std::chrono::high_resolution_clock::now();
        for(dist_fptr dist: distributions){


            std::vector<value_t> items = dist(arrSize);
            testSorting(items, 2);


            int j = 0;
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
        std::cout << milis << "ms" << '\n';
        arrSize *= 2;
    }


    return 0;
}