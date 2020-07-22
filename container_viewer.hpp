/*
    ContainerViewer - class which provides to iterate at same time some containers.

Implementations:
    ContainerViewer<no_wrapper: default> iterates until the least container finishes.
    ContainerViewer<optional/unique_ptr> iterates until the biggest container finishes but
                                         filling in with the neutral element 'nullopt'/'nullptr'.
*/
#ifndef QoUtils_CONTAINER_VIEWER_HPP
#define QoUtils_CONTAINER_VIEWER_HPP

#include <tuple>

#include "type_traits.hpp"
#include "map.hpp"

#include <optional>
#include "tuple_iterator.hpp"

namespace QoUtils
{

template<class> struct value;

template<template<class> class C = value, class ...A>
class ContainerViewer
{
    std::tuple<A&...> m_containers;

    template<class    T> struct get_iterator { using type = typename T::iterator; };
    template<class ...T> struct get_iterator<std::tuple<T...>> { using type = TupleIterator<T...>; };

public:
    ContainerViewer(A& ...containers) : m_containers{ containers... } { ; }

public:
    struct iterator : std::tuple<typename get_iterator<std::remove_reference_t<A>>::type ...>
    {
        constexpr iterator& operator ++() noexcept
        {
            auto& lvalue = *this;
            QoUtils::map([](auto && a){ ++a; }, lvalue);
            return *this;
        }
        constexpr iterator& operator ++(int) noexcept
        {
            auto it = *this;
            operator++();
            return it;
        }

        constexpr iterator& operator --() noexcept
        {
            auto& lvalue = *this;
            QoUtils::map([](auto && a){ --a; }, lvalue);
            return *this;
        }
        constexpr iterator& operator --(int) noexcept
        {
            auto it = *this;
            operator--();
            return it;
        }

        constexpr auto operator*() const noexcept
        {
            if constexpr (sizeof...(A) == 1)
            {
                if constexpr (is_similar_v<C, value>)
                    return *std::get<0>(*this);
                else
                    return *std::get<0>(*this);
            }
            else
            {
                if constexpr (is_similar_v<C, value>)
                    return QoUtils::map([](auto && a) { return *a; }, *this);
                else
                    return QoUtils::map([](auto && a) { return C{*a}; }, *this);
            }
        }

//        constexpr bool operator ==(const iterator& it) noexcept
//        {
//            if constexpr (is_similar_v<C, value>)
//            {
//                bool end = false;
//                for (auto [a, b] : covi(*this, it))
//                {
//                    if(a == b)
//                    {
//                        end = true;
//                        break;
//                    }
//                }

//                return end;
//            }
//        }
//        constexpr bool operator !=(const iterator& it) noexcept
//        {
//            return !operator==(it);
//        }
    };

    constexpr auto begin() const noexcept
    {
        auto && t = map([](auto && a)
        {
            if constexpr (is_tuple_v<std::remove_reference_t<decltype (a)> >)
                return QoUtils::begin(a);
            else
                return a.begin();
        }, m_containers);

        return iterator { std::move(t) };
    }
    constexpr auto end()   const noexcept
    {
        auto && t = map([](auto && a)
        {
            if constexpr (is_tuple_v<std::remove_reference_t<decltype (a)> >)
                return QoUtils::end(a);
            else
                return a.end();
        }, m_containers);

        return iterator { std::move(t) };
    }
};

template<template<class> class C = value, class ...A>
auto covi(A && ...a)
{
    return ContainerViewer<C, A...>(a ...);
}

}

#endif // QoUtils_CONTAINER_VIEWER_HPP
