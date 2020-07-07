#ifndef MAP_HPP
#define MAP_HPP

namespace QoUtils
{

#include <vector>
#ifdef _GLIBCXX_VECTOR

template<class T, class F>
std::vector<T> map(F && f, const std::vector<T>& v)
{
    std::vector<T> result(v.size());
    const std::size_t size = v.size();

    for (std::size_t i = 0; i < size; ++i)
    {
        result[i] = f(v[i]);
    }

    return result;
}

template <class T, class F>
std::vector<T>& map(F && f, std::vector<T>& v)
{
    constexpr bool void_return = std::is_void_v<decltype(f(std::declval<T>()))>;

    for (auto && item : v)
    {
        if constexpr (void_return)
        {
            f(item);
        }
        else {
            item = f(item);
        }
    }

    return v;
}

#endif

}
#endif // MAP_HPP
