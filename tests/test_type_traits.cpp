
#include "module.hpp"

#include <tuple>
#include <array>
#include <vector>
#include <variant>
#include "type_traits.hpp"

#include "tuple_iterator.hpp"

BOOST_AUTO_TEST_SUITE(test_type_traits);

#define CREATE_OBJECT_AND_REFERENCES(container, ...)      \
static std::container<__VA_ARGS__> container;              \
static       auto&&  r_##container = std::move(container); \
static const auto&& cr_##container = std::move(container); \
static       auto&   l_##container = container;            \
static const auto&  cl_##container = container;            \
                                                           \
static_assert (std::is_lvalue_reference_v<decltype( l_##container)>); \
static_assert (std::is_lvalue_reference_v<decltype(cl_##container)>); \
static_assert (std::is_rvalue_reference_v<decltype( r_##container)>); \
static_assert (std::is_rvalue_reference_v<decltype(cr_##container)>);

CREATE_OBJECT_AND_REFERENCES(array,   int, 3);
CREATE_OBJECT_AND_REFERENCES(vector,  int);
CREATE_OBJECT_AND_REFERENCES(tuple,   int, bool);
CREATE_OBJECT_AND_REFERENCES(variant, int, double);

template<template<class ...> class T, class ...XS> constexpr
auto check_not(XS ...) noexcept
{
    static_assert ( (!std::is_reference_v<XS> && ...) );
    static_assert ( (!T<XS>::value && ...) );
}

template<template<class ...> class T, class ...XS> constexpr
auto check_ref_not(XS && ...) noexcept
{
    static_assert ( (std::is_reference_v<XS> && ...) );
    static_assert ( (!T<XS>::value && ...) );
}

BOOST_AUTO_TEST_CASE(test_tuple)
{
    static_assert ( is_tuple_v<decltype(tuple)> );

    check_not<is_tuple>(array, vector, variant);
    check_ref_not<is_tuple>(
                r_tuple,    l_tuple,    cr_tuple,    cl_tuple,
                r_vector,   l_vector,   cr_vector,   cl_vector,
                r_array,    l_array,    cr_array,    cl_array,
                r_variant,  l_variant,  cr_variant,  cl_variant
                           );
}

BOOST_AUTO_TEST_SUITE_END();
