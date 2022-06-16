//
// Created by James Marsden on 6/15/22.
//

#ifndef CTUT_FIXTURE_CONECEPT_H
#define CTUT_FIXTURE_CONECEPT_H

namespace lib{

    inline namespace ctut_1_0{
        template<typename Fixture_T>
        concept Fixture_Concept = requires(Fixture_T fixture)
        {
            {fixture.function};
            {fixture.test_name};
            {fixture.test_state};
        };
    }
}





#endif //CTUT_FIXTURE_CONECEPT_H
