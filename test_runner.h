//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_RUNNER_H
#define CTUT_TEST_RUNNER_H

#include "test_invocable_concept.h"
#include "test_fixture.h"
#include "fixture_conecept.h"
#include <array>
#include <algorithm>
#include <iostream>

namespace lib {

    inline namespace ctut_1_0 {


        namespace private_ns {
            constexpr bool run_single_test_ceval(TestableAndCallable auto function, auto &&... args) {
                return function(args...);
            }
        }

        constexpr bool run_single_test(Fixture_Concept auto fixture, auto &&... args) {
            using private_ns::run_single_test_ceval;
            if (std::is_constant_evaluated()) {
                return run_single_test_ceval(fixture.function, args...);
            } else {
                std::cout << "Not ran as a constant expression... \n";
                if (auto r = run_single_test_ceval(fixture.function, args...); r) {
                    std::cout << "Test " << fixture.test_name << " passed!\n";
                    return r;
                } else {
                    std::cout << "test failed :c\n";
                    return r;
                }
            }
        }

        template<class Fixture_T, auto size>
        constexpr auto run_multi_test(std::array<Fixture_T, size> const &container, auto &&... args) {

            std::remove_cvref_t<decltype(container)> result;
//            std::array<fixture<Function::function>, size> result{};
            auto end = container.end();
            if (size < container.size()) end = container.begin() + size;

            std::transform(container.begin(), end, result.begin(),
                           [&args...](Fixture_Concept auto &lfixture) -> Fixture_T {

                               return {lfixture.function, lfixture.test_name,
                                       private_ns::run_single_test_ceval(lfixture.function, args...)
                                       ? test_data_enum::passed : test_data_enum::failed};
                           });
            return result;
        }

    } // inline namespace v_1
} // namespace lib

#endif //CTUT_TEST_RUNNER_H
