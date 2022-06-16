//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_FIXTURE_H
#define CTUT_TEST_FIXTURE_H

#include "test_invocable_concept.h"

#include <source_location>

namespace lib {
    inline namespace v_0 { // for ABI protections
        enum struct test_data_enum {
            untested = -1,
            failed = 0,
            passed,
        };

        template <TestableAndCallable Function_T>
        struct fixture{

            std::source_location location{};
            Function_T function{};


            constexpr fixture(Function_T  const & function) : function{function}{}
                              //, std::source_location location = std::source_location::current()) : function{function}, location{location} {}

            constexpr fixture() = default;
            constexpr fixture(fixture const &) = default;
            constexpr fixture(fixture && ) noexcept = default;
            constexpr ~fixture() = default;

            constexpr fixture& operator=(fixture const &) = default;
            constexpr fixture& operator=(fixture &&) noexcept = default;

            [[nodiscard("fucntion")]] constexpr const Function_T &  get_function() const noexcept { return function;}
            constexpr auto operator()(auto ... args){
                return function(args...);
            }

            operator bool(){
                return function != nullptr;
            }

        };
    } // inline namespace v_0
} // namespace lib

#endif //CTUT_TEST_FIXTURE_H
