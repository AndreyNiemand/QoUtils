#ifndef QoUtils_TUPLE_ITERATOR_HPP
#define QoUtils_TUPLE_ITERATOR_HPP

#include <tuple>

namespace QoUtils
{

template<std::size_t I, class ...A>
struct TupleIterator
{
    std::tuple<A...>& m_t;

    constexpr
    TupleIterator<I+1, A...> operator++() noexcept
    {
        return {m_t};
    }

    constexpr
    auto && operator*() const noexcept
    {
        return std::get<I>(m_t);
    }
};

template<class ...A> constexpr
auto begin(std::tuple<A...>& t) noexcept
{
    return TupleIterator<0, A...>{t};
}

template<class ...A> constexpr
auto end(std::tuple<A...>& t) noexcept
{
    return TupleIterator<sizeof... (A), A...>{t};
}

}

#endif // QoUtils_TUPLEITERATOR_HPP
