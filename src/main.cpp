#include "distributions.h"
#include "sorting.h"
#include <iostream>


using value_t = double;
using dist_fptr = distributions::dist_gen_fptr_t<value_t>;

int main(){
    std::pair<std::string, dist_fptr> distributions[] = {
        {"shuffled_ascending",distributions::shuffled_ascending()},
//        {"shuffled_similar",distributions::shuffled_similar()},
//        {"all_equal",distributions::all_equal()},
//        {"ascending",distributions::ascending()},
//        {"descending",distributions::descending()},
//        {"swapped_endpoints",distributions::swapped_endpoints()}
    };
    size_t sizes[] = {
            4, 8, 16, 64, 128
    };

    distributions::seed_prng(0);

    size_t arrSize = 1l << 15;
//    size_t arrSize = 1l << 24;
    for(int i=0; i<1; ++i){
        std::cout << "Size: " << arrSize << '\n';

        for(const auto& distPair: distributions){
            const std::string& distName = distPair.first;
            auto dist = distPair.second;

            std::cout << distName << '\n';

            std::vector<value_t> items = dist(arrSize);
            testSorting(items, 16);


            int j = 0;
        }



        arrSize *= 2;
    }


    return 0;
}