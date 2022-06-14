//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_FIXTURE_H
#define CTUT_TEST_FIXTURE_H

#include "test_invocable_concept.h"

namespace lib {
    inline namespace v_0 { // for ABI protections
        enum struct test_data_enum {
            untested = -1,
            failed = 0,
            passed,
        };
    } // inline namespace v_0
} // namespace lib

#endif //CTUT_TEST_FIXTURE_H
