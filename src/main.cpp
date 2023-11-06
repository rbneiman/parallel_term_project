#include "gpu/algo_test.cuh"
#include "distributions.h"

using value_type = double;
using dist_fptr = distributions::dist_gen_fptr_t<value_type>;

int main(){
    run_algo();

    dist_fptr distributions[] = {
            distributions::shuffled_random(),
        distributions::all_equal(),
        distributions::ascending(),
        distributions::descending(),
        distributions::swapped_endpoints()
    };
    size_t sizes[] = {
            4, 8, 16, 64, 128
    };

    for(size_t size : sizes){

        for(dist_fptr dist: distributions){
            std::vector<value_type> items = dist(size);
            int j = 0;
        }
    }


    return 0;
}