#ifndef _OPERATION_LOG_TYPE_TRAITS_H
#define _OPERATION_LOG_TYPE_TRAITS_H

#include <iostream>
#include <string>
#include <utility>


namespace operation_log
{

// A predicate class to tell whether the function `std::to_string(value)`
// exists for a value of the given type `T`.
template <typename T>
class HasToString
{
    template <
        typename TestedT,
        // This only resolves, if `std::to_string(TestedT())` exists:
        typename = decltype( std::to_string(std::declval<TestedT>()) )>
    static std::true_type has_trait(int);

    template <typename TestedT>
    static std::false_type has_trait(double);

    public:

    typedef decltype(has_trait<T>(0)) has_trait_type;
    static constexpr bool value = has_trait_type::value;
};

// A predicate class to tell whether the operator `std::cout << value`
// exists for a value of the given type T.
template <typename T>
class HasOstreamRShift
{
    template <
        typename TestedT,
        // This only resolves, if `std::cout << TestedT()` exists:
        typename = decltype( std::cout << std::declval<TestedT>() )>
    static std::true_type has_trait(int);

    template <typename TestedT>
    static std::false_type has_trait(double);

    public:

    typedef decltype(has_trait<T>(0)) has_trait_type;
    static constexpr bool value = has_trait_type::value;
};

}

#endif // _OPERATION_LOG_TYPE_TRAITS_H
