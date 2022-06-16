#include <numeric>
#include <iostream>

#include "test_runner.h"

constexpr bool sanity_check(int) { return true; }

constexpr bool sanity_check_fail(int) { return false; }

constexpr bool overflow_test(int) {
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

constexpr bool sanity_check_new_delete(int) {
    int *i = new int;
    delete i;
    return true;
}

constexpr bool memory_leak(int) {
    int *i = new int;
    return true;
}

int main() {


    using function_t = bool (*)(int);

    constexpr auto r2 = lib::run_multi_test(std::array{
            lib::fixture<function_t>{sanity_check, "Sanity check"},
            lib::fixture<function_t>{sanity_check_fail, "Sanity check fail"},
            lib::fixture<function_t>{sanity_check_new_delete, "Sanity check new/delete"},
//            lib::fixture<function_t>{overflow_test, "overflow test"},
//            lib::fixture<function_t>{out_of_bounds_test, "Out of bounds (Bad test)"},
//            lib::fixture<function_t>{memory_leak, "Memeory Leak"},
    }, 0);

    for (auto const &[e1, e2, e3]: r2) {

        std::cout << "Test name: " << e2 << '\t';
        if (e3 == lib::test_data_enum::passed) std::cout << "test passed \n";
        else if (e3 == lib::test_data_enum::failed) std::cout << "test failed\n";
    }
    std::cout << r2.size() << '\n';
    return 0;
}
