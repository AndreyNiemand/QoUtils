#ifndef QoUtils_CONTAINERVIEWER_HPP
#define QoUtils_CONTAINERVIEWER_HPP

#include <tuple>
#include "map.hpp"

namespace QoUtils
{

template<class ...A>
class ContainerViewer
{
    std::tuple<A&...> m_containers;

public:
    ContainerViewer(A& ...containers) : m_containers{ containers... }
    {    }

public:
    struct iterator : std::tuple<typename A::iterator ...>
    {
        iterator& operator ++() noexcept
        {
            auto& lvalue = *this;
            QoUtils::map([](auto && a){ ++a; }, lvalue);
            return *this;
        }

        auto operator*() const noexcept
        {
            return QoUtils::map([](auto& a) -> std::tuple<typename A::value_type& ...>
            {
                return *a;
            }, *this);
        }
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

}

#endif // QoUtils_CONTAINERVIEWER_HPP
