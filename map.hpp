#ifndef QoUtils_MAP_HPP
#define QoUtils_MAP_HPP

/*
    map - a function changing a container with one argrument predicate;

Implementations:
    1) con<T> = map(_, const &);
    2) con<T> = map(_,       &);
    3) con<T> = map(_,      &&);

    4) con<const T&> map(f, _); const T& f(_);
    5) con<      T&> map(f, _);       T& f(_);
    6) con<     T&&> map(f, _);      T&& f(_);

   *7) void map(f, _); void f(_);

Supported containers:
    vector,
    tuple; (7) if f(_) -> void, there will be no item in the place of result tuple;

Examples:
    1)
        const a = {1, 2, 3, 4};
        r = map ([](auto b){ return b+2; },  a);
        r == {3, 4, 5, 6}

    2)
        a = {1, 2, 3, 4};
        map ([](auto& b){ b*=2; return b+2; },  a) == {3, 4, 5, 6};
        a == {2, 4, 6, 8};

    3)
        a = {"We", "are", "string"}; // Container with std::string;
        r = map([](auto b){ return UpperCase(b); }, std::move(a));
        r == { "WE", "ARE", "STRING" };
        a == { "", "", "" };

   *7)
        tuple = {1,2,"3","4"};
        f = [](a){ if constexpr (is_integral<decltype(a)>) return a; };
        map(f, tuple) == {1,2};
*/

#include <def_guard.hpp>

namespace QoUtils
{

#if QoUtils_VECTOR

template<class T, class F> constexpr
auto map(F && f, const std::vector<T>& v) noexcept(std::is_nothrow_invocable_v<F, T>)
{    
    std::vector<decltype(f(std::declval<T>()))> result(v.size());
    const std::size_t size = v.size();

    for (std::size_t i = 0; i < size; ++i)
    {
        result[i] = f(v[i]);
    }

    return result;
}

template <class T, class F> constexpr
auto& map(F && f, std::vector<T>& v) noexcept(std::is_nothrow_invocable_v<F, T>)
{
    constexpr bool void_return = std::is_void_v<decltype(f(std::declval<T>()))>;

    for (auto && item : v)
    {
        if constexpr (void_return)
        {
            f(item);
        } else {
            item = f(item);
        }
    }

    return v;
}

template <class T, class F> constexpr
auto map(F && f, std::vector<T>&& v) noexcept(std::is_nothrow_invocable_v<F, T>)
{
    std::vector<decltype(f(std::declval<T>()))> result(v.size());
    const std::size_t size = v.size();

    for (std::size_t i = 0; i < size; ++i)
    {
        result[i] = f(std::move(v[i]));
    }

    return result;
}

#endif

#if QoUtils_TUPLE

namespace detail::tuple
{

template<std::size_t ...I, class F, class T> constexpr
auto map_impl(F && f, T && t, std::integer_sequence<std::size_t, I...>)
{
    const auto d = [&f](auto && a)
    {
        if constexpr (std::is_void_v<decltype(f(a))>)
        {
            f(a);
            return std::tuple<>();
        }
        else {
            return std::tuple<decltype(f(a))>(f(a));
        }
    };

    return std::tuple_cat( d(std::get<I>(std::forward<T>(t))) ... );
}

}

template<class F, class ...A> constexpr
auto map(F && f, const std::tuple<A...>& t)
{
    return detail::tuple::map_impl(std::forward<F>(f), t,
                                   std::make_index_sequence<sizeof... (A)>{});
}

template<class F, class ...A> constexpr
auto map(F && f, std::tuple<A...>& t)
{
    return detail::tuple::map_impl(std::forward<F>(f), t,
                                   std::make_index_sequence<sizeof... (A)>{});
}

template<class F, class ...A> constexpr
auto map(F && f, std::tuple<A...>&& t)
{
    return detail::tuple::map_impl(std::forward<F>(f), std::move(t),
                                   std::make_index_sequence<sizeof... (A)>{});
}

#endif

}
#endif // QoUtils_MAP_HPP
