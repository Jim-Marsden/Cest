//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_FIXTURE_H
#define CTUT_TEST_FIXTURE_H

#include "test_invocable_concept.h"

#include <string_view>

namespace lib {

    inline namespace ctut_1_0{
        enum struct test_data_enum {
            untested = 0,
            failed = 1,
            passed,
        };
        template<TestableAndCallable Function_T>
        struct fixture{
            Function_T function;
            std::string_view test_name{};
            test_data_enum test_state{};
        };
    }

} // namespace lib

#endif //CTUT_TEST_FIXTURE_H
