#ifndef QoUtils_TUPLE_ITERATOR_HPP
#define QoUtils_TUPLE_ITERATOR_HPP

/*
    TupleIterator - class which allows to iterate through any tuple's elements and change them.
    // TupleConstIterator - same functions but no able to change the items;

    Observers:
        Iterator operator[];

        std::variant<T1, T2, ...> operator*();
        // operator T&() throw(bad_cast);
        operator T*();
        // operator std::optional<T>();

        [T&, bool]        get<T>(it);
        std::optional<T&> get<std::optional<T>>(it);
        T                 get_common_of<T1, T2, ...>(it);

        // [T&&, bool]     move<T>(it);

    Modifiers:
        operators ++, --;

    Comparers:
        operators ==, !=, <=>;
*/

#include <tuple>

#include "def_guard.hpp"
#include "type_traits.hpp"

namespace QoUtils
{

template<class ...T>
struct TupleIterator
{
    using iterator_category = std::random_access_iterator_tag;

    using difference_type = std::size_t;

    constexpr auto && operator++()    noexcept { ++m_i; return *this; }
    constexpr auto    operator++(int) noexcept { auto t = *this; operator++(); return t; }

    constexpr auto && operator--()    noexcept { --m_i; return *this; }
    constexpr auto    operator--(int) noexcept { auto t = *this; operator--(); return t; }

    constexpr auto && operator[](std::size_t i) const
    {
        return TupleIterator<T...>{r_t, m_i + i}.operator*();
    }

#if QoUtils_VARIANT
    template<std::size_t I = 0> constexpr
    std::variant<T...> operator*() const noexcept
    {
        if constexpr (I + 1 < sizeof...(T))
            if (m_i != I)
                return this->template operator*<I+1>();

        return std::get<I>(r_t);
    }
#endif

    constexpr bool operator ==(const TupleIterator<T...>& t) const noexcept { return (m_i == t.m_i) && (&r_t == &t.r_t); }
    constexpr bool operator !=(const TupleIterator<T...>& t) const noexcept { return !operator==(t); }

    template<class R> constexpr
    operator R*() const noexcept
    {
        return get_impl<R>();
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
        else return nullptr;
    }

    TupleIterator(std::tuple<T...>& a, std::size_t i): r_t{a}, m_i{i} {}

    std::tuple<T...>& r_t;
    std::size_t m_i;

    template<class ...B> friend constexpr auto begin(std::tuple<B...>&) noexcept;
    template<class ...B> friend constexpr auto end  (std::tuple<B...>&) noexcept;
    template<class R, class ...A>              constexpr friend auto get(TupleIterator<A...> it) noexcept;
    template<class R, class ...RS, class ...A> constexpr friend std::pair<std::common_type_t<R, RS...>, bool> get_common_of(TupleIterator<A...> it);
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

template<class R, class ...RS, class ...A> constexpr
std::pair<std::common_type_t<R, RS...>, bool> get_common_of(const TupleIterator<A...> it)
{
    if (auto v = it.template get_impl<R>())
    {
        return { *v, true };
    }

    if constexpr (sizeof... (RS) != 0)
    {
        return get_common_of<RS...>(it);
    }

    return {std::common_type_t<R, RS...>{ }, false};
}

template<class R, class ...A> constexpr
auto get(const TupleIterator<A...> it) noexcept
{
#if QoUtils_OPTIONAL
    if constexpr (is_optional_v<R>)
    {
        using value_type = typename R::value_type;
        auto v = it.template get_impl<value_type>();
        return v ? std::optional<std::reference_wrapper<value_type> >
            { *v } : std::nullopt;
    }
    else
#endif
    {
        bool stub = false;
        auto v = it.template get_impl<R>();
        return v ? std::pair<R&, bool> {                         *v,  true }
                 : std::pair<R&, bool> { reinterpret_cast<R&>(stub), false };
    }
}


}

#endif // QoUtils_TUPLEITERATOR_HPP
