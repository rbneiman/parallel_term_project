#include "distributions.h"
#include "sorting.h"
#include <iostream>


using value_t = int32_t ;
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

    std::cout << "[\n";
//    size_t arrSize = 1l << 11;
    for(const auto& distPair: distributions){
        const std::string& distName = distPair.first;
        auto dist = distPair.second;

        std::cout << "\n{\n\"Dist\": \""<< distName << "\",\n\"results\": [\n";

        for(int i=4; i<22; ++i){
            size_t arrSize = 1l << i;
            std::cout << "{\n\"Size\": " << arrSize << ",\n\"results\": [";

            std::vector<value_t> items = dist(arrSize);
            testSorting(items, 8);


            int j = 0;
            std::cout << "\n]\n},\n";
            arrSize *= 2;
        }


        std::cout << "\n]\n},\n";

    }
    std::cout << "]";

    return 0;
}