#ifndef QoUtils_CONTAINER_VIEWER_HPP
#define QoUtils_CONTAINER_VIEWER_HPP

#include <tuple>
#include "map.hpp"

#include <optional>
#include "type_traits.hpp"

namespace QoUtils
{

template<class> struct value;

template<template<class> class C = value, class ...A>
class ContainerViewer
{
    std::tuple<A&...> m_containers;

public:
    ContainerViewer(A& ...containers) : m_containers{ containers... }
    {    }

public:
    struct iterator : std::tuple<typename std::remove_reference_t<A>::iterator ...>
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
            if constexpr (is_similar_v<C, value>)
                return QoUtils::map([](auto && a) { return *a; }, *this);
            else
                return QoUtils::map([](auto && a) { return C{*a}; }, *this);
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
        return iterator { map([](auto && a){ return a.begin(); }, m_containers) };
    }
    constexpr auto end()   const noexcept
    {
        return iterator { map([](auto && a){ return a.end  (); }, m_containers) };
    }
};

template<template<class> class C = value, class ...A>
auto covi(A && ...a)
{
    return ContainerViewer<C, A...>(a ...);
}

}

#endif // QoUtils_CONTAINER_VIEWER_HPP
