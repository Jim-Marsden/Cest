//
// Created by James Marsden on 6/13/22.
//

#ifndef CTUT_TEST_INVOCABLE_CONCEPT_H
#define CTUT_TEST_INVOCABLE_CONCEPT_H

#include <concepts>
namespace lib {

    inline namespace ctut_1_0{
        template<typename Type, typename ... Parameter_Pack>
        concept TestableAndCallable = requires(Type object, Parameter_Pack &&... parameterPack)
        {
            { static_cast<bool>(object) } -> std::convertible_to<bool>;
            { std::invocable<Type> } -> std::convertible_to<bool>;
        };

    } // inline namespace v_0
} // namespace lib

#endif //CTUT_TEST_INVOCABLE_CONCEPT_H
