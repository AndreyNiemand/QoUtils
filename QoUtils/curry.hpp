/*
    curry - trasform a tuple-arguments function into one-argument function to one-argument function...

Examples:
    int f(int, double){ ... }
    auto cf = curry(f);
    ... = cf(1)(1.0);
*/

#ifndef QoUtils_CURRY_HPP
#define QoUtils_CURRY_HPP

namespace QoUtils
{

namespace detail {
    template<std::size_t N, class F, class ...A>
    auto curry_impl(F f, A &&...a)
    {
        if constexpr (N != 0)
        {
            return [=](auto x){ return curry_impl<N-1>(f, a..., x); };
        }
        else
        {
            return f(a...);
        }
    }
}

template<class R, class...A>
auto curry(R (f)(A...))
{
    return [=](auto x){ return detail::curry_impl<sizeof...(A)-1>(f, x); };
}

} // QoUtils

#endif //QoUtils_CURRY_HPP