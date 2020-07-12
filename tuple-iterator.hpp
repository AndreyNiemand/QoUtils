#ifndef QoUtils_TUPLE_ITERATOR_HPP
#define QoUtils_TUPLE_ITERATOR_HPP

#include <tuple>
#include <variant>

namespace QoUtils
{

template<std::size_t I, class X, class ...XS>
struct get_type_by
{
    using type = typename get_type_by<I-1, XS...>::type;
};

template<class X, class ...XS>
struct get_type_by<0, X, XS...>
{
    using type = X;
};


template<std::size_t I, class ...XS>
using get_type_by_t = typename get_type_by<I, XS...>::type;

template<class ...T>
struct TupleIterator
{
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::variant<T...>;
    using difference_type = std::size_t;

    constexpr auto && operator++()    noexcept { ++m_i; return *this; }
    constexpr auto    operator++(int) noexcept { auto t = *this; operator++(); return t; }

    constexpr auto && operator--()    noexcept { --m_i; return *this; }
    constexpr auto    operator--(int) noexcept { auto t = *this; operator--(); return t; }

    constexpr auto && operator[](std::size_t i) const
    {
        return TupleIterator<T...>{r_t, m_i + i}.operator*();
    }

//    constexpr std::variant<std::reference_wrapper<T>...> operator*() const
//    {
//        return std::get<0>(r_t);

////        throw std::out_of_range{
////               "The tuple contains no item by index: "s + std::to_string(m_i)
////                           + "; (QoUtils_MAX_COUNT_TUPLE_ITEMS = "
////                + std::to_string(QoUtils_MAX_COUNT_TUPLE_ITEMS) + ")"};

//    }

    constexpr bool operator ==(const TupleIterator<T...>& t) const noexcept { return (m_i == t.m_i) && (&r_t == &t.r_t); }
    constexpr bool operator !=(const TupleIterator<T...>& t) const noexcept { return !operator==(t); }

//    template<class R, class ...RS> constexpr
//    bool get(R *result, RS* ...) const noexcept
//    {
//        if constexpr (sizeof... (RS) != 0)
//        {
//            if (result = get_impl<R, 0>())
//                return result;
//            else
//                return get<RS...>();
//        }

//        return false;
//    }

    template<class R>
    operator R*() const noexcept
    {
        return get_impl<R, 0>();
    }

private:
    template<class R, std::size_t I = 0> constexpr
    R* get_impl() const noexcept
    {
        if constexpr (I < sizeof... (T))
        {
            if constexpr (std::is_same_v<R, get_type_by_t<I, T...> >)
                if(m_i == I)
                    return &std::get<I>(r_t);

            return get_impl<R, I+1>();
        }

        return nullptr;
    }

    TupleIterator(std::tuple<T...>& a, std::size_t i): r_t{a}, m_i{i} {}

    std::tuple<T...>& r_t;
    std::size_t m_i;

    template<class ...B> friend constexpr auto begin(std::tuple<B...>&) noexcept;
    template<class ...B> friend constexpr auto end  (std::tuple<B...>&) noexcept;
};

template<class ...A> constexpr
auto begin(std::tuple<A...>& t) noexcept
{
    return TupleIterator<A...>{t, 0};
}

template<class ...A> constexpr
auto end(std::tuple<A...>& t) noexcept
{
    constexpr auto size = sizeof...(A);

    return TupleIterator<A...>{t, size};
}

}

#endif // QoUtils_TUPLEITERATOR_HPP
