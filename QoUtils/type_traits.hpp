#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include <type_traits>
#include "def_guard.hpp"

#if QoUtils_OPTIONAL
template<class T> struct is_optional                   : std::false_type { };
template<class T> struct is_optional<std::optional<T>> : std::true_type  { };
template<class T> constexpr bool is_optional_v = is_optional<T>::value;
#endif

#if QoUtils_VECTOR
template<class T> struct is_vector                 : std::false_type { };
template<class T> struct is_vector<std::vector<T>> : std::true_type  { };
template<class T> constexpr bool is_vector_v = is_vector<T>::value;
#endif

#if QoUtils_TUPLE
template<class ...T> struct is_tuple                   : std::false_type { };
template<class ...T> struct is_tuple<std::tuple<T...>> : std::true_type  { };
template<class ...T> constexpr bool is_tuple_v = is_tuple<T...>::value;
#endif


template<std::size_t I, class X, class ...XS>
struct get_type_by { using type = typename get_type_by<I-1, XS...>::type; };

template<class X, class ...XS>
struct get_type_by<0, X, XS...> { using type = X; };

template<std::size_t I, class ...XS>
using get_type_by_t = typename get_type_by<I, XS...>::type;


template<template<class...> class, template<class...> class>
struct is_similar : std::false_type{};

template<template<class...> class T>
struct is_similar<T, T> : std::true_type{};

template<template<class...> class T, template<class...> class D>
constexpr bool is_similar_v = is_similar<T, D>::value;

#endif // TYPE_TRAITS_HPP
