#include "test_runner.h"
#include <numeric>
#include <iostream>

constexpr bool sanity_check(int) { return true; }

constexpr bool overflow_test(int){
    auto max = std::numeric_limits<int>::max();
    max = max + max + 1;
    return max > std::numeric_limits<int>::max();
}


constexpr bool out_of_bounds_test(int) {
    try {
        std::array<int, 1> out_of_bounds{};
        out_of_bounds[2] = 3;
    } catch (...) {
        return true;
    }
    return false;
}

constexpr bool sanity_check_new_delete(int){
    int *i = new int;
    delete i;
    return true;
}


constexpr bool memory_leak(int){
    int *i = new int;
    return true;
}

int main() {

    constexpr static lib::Tests<bool (*)(int)> test_manager{sanity_check, sanity_check_new_delete,
//        memory_leak,
//        out_of_bounds_test,
//        overflow_test
        [](int){return true;}
        };

    static constinit auto test_results = test_manager.run_test(3);

    int tests_passed{};
    int tests_failed{};
    for(auto const & e: test_results ){
        if(e == lib::test_data_enum::passed){
            tests_passed++;
        }
        else if(e == lib::test_data_enum::failed){
            tests_failed++;
        }
    }
    std::cout << "Passed: " << tests_passed << "\tFailed: " << tests_failed << "\t total: " << test_results.size();

    return 0;
}
