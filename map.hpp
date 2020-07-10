#ifndef QoUtils_MAP_HPP
#define QoUtils_MAP_HPP

/*
    map - a function changing a container with one argrument predicate;

Implementations:
    1) con<T> = map(_, const &);
    2) con<T> = map(_,       &);
    3) con<T> = map(_,      &&);

    4) map(f, _) = delete; const T& f(D);
    5) map(f, _) = delete;       T& f(D);
    6) map(f, _) = delete;      T&& f(D);

Supported containers:
    vector,
    tuple;

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
*/

namespace QoUtils
{

//#include <vector>
#ifdef _GLIBCXX_VECTOR

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

//#include <tuple>
#ifdef _GLIBCXX_TUPLE

namespace detail { namespace tuple {

template<std::size_t I, class R, class T, class F> constexpr
void map(R& result, T && t, F && f)
{
    if constexpr(std::is_void_v<std::invoke_result_t<F, decltype(std::get<I>(std::forward<T>(t)))>>)
    {
        f(std::get<I>(std::forward<T>(t)));
    }
    else
    {
        std::get<I>(result) = f(std::get<I>(std::forward<T>(t)));
    }
    if constexpr (I > 0)
        detail::tuple::map<I-1> (result, t, f);
}

}}

template<class F, class ...A> constexpr
auto map(F && f, const std::tuple<A...>& t)
{
    static_assert (!(std::is_reference_v<std::invoke_result_t<F, A>> && ...),
                   "Predicate of map should return no reference.");

    std::tuple<std::invoke_result_t<F, A>...> result;
    detail::tuple::map<sizeof...(A)-1> (result, t, f);
    return result;
}

template<class F, class ...A> constexpr
auto map(F f, std::tuple<A...>& t)
{
    static_assert (!(std::is_reference_v<std::invoke_result_t<F, A>> && ...),
                   "Predicate of map should return no reference.");

    std::tuple<std::invoke_result_t<F, A>...> result;
    detail::tuple::map<sizeof...(A)-1> (result, t, f);
    return result;
}

template<class F, class ...A> constexpr
auto map(F && f, std::tuple<A...>&& t)
{
    static_assert (!(std::is_reference_v<std::invoke_result_t<F, A>> && ...),
                   "Predicate of map should return no reference.");

    std::tuple<std::invoke_result_t<F, A>...> result;
    detail::tuple::map<sizeof...(A)-1> (result, std::move(t), f);
    return result;
}

#endif

}
#endif // QoUtils_MAP_HPP
