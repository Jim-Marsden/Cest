//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_RUNNER_H
#define CTUT_TEST_RUNNER_H

#include "test_invocable_concept.h"
#include "test_fixture.h"
#include <array>
#include <algorithm>
#include <source_location>
#include <iostream>

namespace lib {

    namespace v_3{
        namespace {
            constexpr bool run_single_test_ceval(TestableAndCallable auto function, auto && ... args){
                return function(args...);
            }
        }
        constexpr bool run_single_test(TestableAndCallable auto function, std::source_location location, auto && ... args){
            if (std::is_constant_evaluated()) {
                return run_single_test_ceval(function, args...);
            }
            else {
                std::cout << "Not ran as a constant expression... \n";
                if(auto r = run_single_test_ceval(function, args...); r ) {
                    std::cout << "file: "
                              << location.file_name() << "("
                              << location.line() << ":"
                              << location.column() << ") `"
                              << location.function_name() << "passed!\n";
                    return r;
                }
                else{
                    std::cout << "test failed :c\n";
                    return r;
                }
            }
        }
    }
    namespace v_2{

        template<class T, auto S = 0xff>
        using Test_Containers = std::array<fixture<T>, S>;

        template <TestableAndCallable Function_T, std::size_t Size = 0xff, class ... Aggs_T>
        consteval auto run_tests(Test_Containers<Function_T, Size> const &functions, Aggs_T && ... args){

            std::array<test_data_enum, Size> result{};

            functions.front().function(args...);

//            if constexpr(std::is_constant_evaluated()) {
//                std::transform(functions.begin(), functions.end(), result.begin(), [&args...](fixture<Function_T> function)->test_data_enum{
//                    //if(function != nullptr) return function(args...) ? test_data_enum::passed : test_data_enum::failed;
//
//                    return test_data_enum::untested;
////                    if(function.function != nullptr) // temp solution to what seems to be an issue on msvc with constexpr.
////                        return{};//return function.function(args...) ? test_data_enum::passed : test_data_enum::failed;
////                    return test_data_enum::untested;
//                });
//            }
            return result;
        }
    }
    inline namespace v_1 { // for ABI protections

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

            explicit constexpr Tests(TestableAndCallable auto &&... functions): elements{functions ...} {}

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
                if constexpr(std::is_constant_evaluated()) {
                    std::transform(elements.begin(), elements.end(), result.begin(), [pack...](auto & function)->test_data_enum{
                        if(function != nullptr) // temp solution to what seems to be an issue on msvc with constexpr.
                            return function(pack...) ? test_data_enum::passed : test_data_enum::failed;
                        return test_data_enum::untested;
                    });
                }
                return result;
            }

        };
    } // inline namespace v_1
} // namespace lib

#endif //CTUT_TEST_RUNNER_H
