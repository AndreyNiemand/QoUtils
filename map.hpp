#ifndef MAP_HPP
#define MAP_HPP

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

}
#endif // MAP_HPP
