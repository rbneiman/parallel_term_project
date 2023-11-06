#ifndef PARALLEL_TERM_PROJECT_DISTRIBUTIONS_H
#define PARALLEL_TERM_PROJECT_DISTRIBUTIONS_H
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>
#include <utility>

namespace distributions{
    std::mt19937_64 prng(std::time(nullptr));

    void seed_prng(std::mt19937_64::result_type seed){
        prng.seed(seed);
    }

    template<typename ItemType>
    using dist_gen_fptr_t = std::vector<ItemType>(*)(size_t);

    template<typename SubType>
    struct distribution_base{

        template<typename ItemType>
        operator dist_gen_fptr_t<ItemType>() const{
            return [](size_t size) -> std::vector<ItemType>{
                return SubType{}.template operator()<ItemType>(size);
            };
        }
    };


    struct shuffled_random: distribution_base<shuffled_random>{
        static constexpr const char* name = "shuffled_random";

        template<typename ItemType>
        std::vector<ItemType> operator()(size_t size) const{
            std::vector<ItemType> out{};
            out.reserve(size);

            for(size_t i=0; i<size; ++i){
                out.emplace_back(static_cast<ItemType>(i));
            }
            std::shuffle(out.begin(), out.end(), prng);

            return out;
        }
    };


    struct shuffled_similar: distribution_base<shuffled_similar>{
        static constexpr const char* name = "shuffled_similar";

        template<typename ItemType>
        std::vector<ItemType> operator()(size_t size){
            std::vector<ItemType> out{};
            out.reserve(size);

            for(size_t i=0; i<size; ++i){
                out.emplace_back(static_cast<ItemType>(i%16));
            }
            std::shuffle(out.begin(), out.end(), prng);

            return out;
        }
    };

    struct all_equal: distribution_base<all_equal>{
        static constexpr const char* name = "all_equal";

        template<typename ItemType>
        std::vector<ItemType> operator()(size_t size){
            std::vector<ItemType> out{};
            out.reserve(size);

            for(size_t i=0; i<size; ++i){
                out.emplace_back(static_cast<ItemType>(1));
            }

            return out;
        }
    };

    struct ascending: distribution_base<ascending>{
        static constexpr const char* name = "ascending";

        template<typename ItemType>
        std::vector<ItemType> operator()(size_t size){
            std::vector<ItemType> out{};
            out.reserve(size);

            for(size_t i=0; i<size; ++i){
                out.emplace_back(static_cast<ItemType>(i));
            }

            return out;
        }
    };

    struct descending: distribution_base<descending>{
        static constexpr const char* name = "descending";

        template<typename ItemType>
        std::vector<ItemType> operator()(size_t size){
            std::vector<ItemType> out{};
            out.reserve(size);

            for(size_t i=size-1; i>0; --i){
                out.emplace_back(static_cast<ItemType>(i));
            }
            out.emplace_back(static_cast<ItemType>(0));

            return out;
        }
    };

    struct swapped_endpoints: distribution_base<swapped_endpoints>{
        static constexpr const char* name = "swapped_endpoints";

        template<typename ItemType>
        std::vector<ItemType> operator()(size_t size){
            std::vector<ItemType> out{};
            out.reserve(size);

            for(size_t i=0; i<size; ++i){
                out.emplace_back(static_cast<ItemType>(i));
            }
            ItemType val = out[size-1];
            out[size-1] = out[0];
            out[0] = val;

            return out;
        }
    };

}
#endif //PARALLEL_TERM_PROJECT_DISTRIBUTIONS_H
