
/* Hello 219! I'll be writting a c++ compile-time
 * unit test that should catch UB, and memory
 * leaks. If they fail, it won't compile
 *
 * I am writing this fresh for your posting.
 */

#include <concepts>
#include <array> // Are handily constexpr friendly
#include <type_traits> // For contexpr checks, because I'm building on c++20, as if consteval is not available on all platforms
#include <iostream>
#include <functional>
#include <numeric>

namespace lib {
    inline namespace v_0 { // for ABI protections

        template<typename Type, typename ... Parameter_Pack>
        concept TestableAndCallable = requires(Type object, Parameter_Pack &&... parameterPack)
        {
            { static_cast<bool>(object) } -> std::convertible_to<bool>;
            { std::invocable<Type> } -> std::convertible_to<bool>;
        };

        enum struct test_data_enum {
            untested = -1,
            failed = 0,
            passed,
        };

        template<TestableAndCallable  test_function, auto array_size = 0xff>
        class Tests {


            std::array<test_function, array_size> elements{};
        public:
            constexpr Tests() = default;

            constexpr Tests(Tests const &) = default;

            constexpr Tests(Tests &&) noexcept = default;

            constexpr ~Tests() = default;

            constexpr Tests &operator=(Tests const &) = default;

            constexpr Tests &operator=(Tests &&) noexcept = default;

            template<typename ...  T>
            constexpr Tests(T &&... functions): elements{functions ...} {}

            constexpr auto add_test(test_function const &testFunction) -> bool {
                for (auto &element: elements) {
                    if (!element) {
                        element = testFunction;
                        return true;
                    }
                }
                return false;
            }

            [[nodiscard("test results")]] consteval auto run_test(auto &&...pack) const noexcept {
                std::array<test_data_enum, array_size> result{};

                auto result_itr = result.begin();
                auto element_itr = elements.begin();
                while (result_itr != result.end() && element_itr != elements.end()) {
                    if (*element_itr) {
                        const auto r = std::invoke(*element_itr, pack...);
                        *result_itr = r ? test_data_enum::passed : test_data_enum::failed;
                    } else *result_itr = test_data_enum::untested;
                    result_itr++;
                    element_itr++;
                }
                return result;
            }

        };
    } // inline namespace v_0
} // namespace lib

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
                                                            memory_leak,
                                                            out_of_bounds_test,
                                                            overflow_test
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
