/*
    zip - a function letting to iterate several containers meanwhile;

Implementations:
   
Supported containers:
    
Examples:
   
*/

#ifndef QoUtils_ZIP_HPP
#define QoUtils_ZIP_HPP

#include <tuple> 

#include "STL_forward_declaration.hpp"
#include "map.hpp"

namespace QoUtils
{
    template<class T, class ...TS>
    class Zipped
    {
        std::tuple<T, TS...> m_slices;

    public:
        Zipped(T && t, TS && ...ts): m_slices(t, ts...) {  }

        template<class _T, class ..._TS>
        class iterator
        {
        public:
            using value_type = std::tuple<_T, _TS...>;

        protected:
            value_type m_iters; 

        public:
            iterator(value_type iters): m_iters(iters) {}
            iterator(iterator const& iter): m_iters(iter.m_iters) {}

            iterator& operator=(iterator const& it) { m_iters = it.m_iters; return *this; }

            iterator& operator++()    { map([](auto && item) { return item++; }, m_iters); return *this; }
            iterator  operator++(int) { auto temp = *this; operator++(); return temp; }

            bool operator==(iterator const& it) const { return m_iters == it.m_iters; }
            bool operator!=(iterator const& it) const { return !operator==(it); };

            auto operator*() const { return map([](auto && item) -> auto && { return *item; }, m_iters); }
        };

        template<class _T, class ..._TS>
        class const_iterator : public iterator<_T, _TS...>
        {
        public:
            using iterator::iterator;
            const_iterator(iterator const& iter): iterator(iter) {}

            const_iterator(value_type iters): iterator(iters) {}
            const_iterator& operator=(const_iterator const& it) { return static_cast<const_iterator&>(iterator::operator=(it)); }

            const_iterator& operator++()      { return static_cast<const_iterator&>(iterator::operator++()); }
            const_iterator  operator++(int _) { return iterator::operator++(_); }

            bool operator==(const_iterator const& it) const { return iterator::operator==(it); }
            bool operator!=(const_iterator const& it) const { return iterator::operator!=(it); };
        };

        auto begin() const
        {
            return iterator(map([](auto && slice){ return slice.first; }, m_slices));
        }
        auto end() const
        {
            return iterator(map([](auto && slice){ return slice.second; }, m_slices));
        }

        auto cbegin() const
        {
            return const_iterator(map([](auto && slice){ return slice.first; }, m_slices));
        }
        auto cend() const
        {
            return const_iterator(map([](auto && slice){ return slice.second; }, m_slices));
        }
    };

    template<class T>
    auto slice(T && t)
    {
        return std::pair{ t.begin(), t.end() };
    }

    template<class T, class ...TS>
    auto zip(T && t, TS && ...ts)
    {
        return Zipped(slice(t), slice(ts)...);
    } 
} // namespace zip

#endif QoUtils_ZIP_HPP