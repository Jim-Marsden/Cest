//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_RUNNER_H
#define CTUT_TEST_RUNNER_H

#include "test_invocable_concept.h"
#include "test_fixture.h"
#include <array>
#include <algorithm>

namespace lib {
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
                        if(function != nullptr)
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
