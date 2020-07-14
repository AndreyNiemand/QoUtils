/*
    With QoUtils_X macro's help, you can define additional functions in .hpp files.

    Example:
        #include <tuple>           // or #define QoUtils_TUPLE 1
        #define QoUtils_OPTIONAL 1 // or #include <optional>

        #include <map.hpp>
        // all it added the functions: map(std::tuple) and map(std::optional).
*/

#ifndef QoUtils_DEF_GUARD_HPP
#define QoUtils_DEF_GUARD_HPP

#define QoUtils_TITLE(x)           \
              QoUtils_ ## x        \
          || _GLIBCXX_ ## x        \
          || _LIBCPP_  ## x

#if QoUtils_TITLE(OPTIONAL)
#include    <optional>
#define     QoUtils_OPTIONAL 1
#else
#define     QoUtils_OPTIONAL 0
#endif

#if QoUtils_TITLE(VECTOR)
#include    <vector>
#define     QoUtils_VECTOR 1
#else
#define     QoUtils_VECTOR 0
#endif

#if QoUtils_TITLE(TUPLE)
#include    <tuple>
#define     QoUtils_TUPLE 1
#else
#define     QoUtils_TUPLE 0
#endif

#if QoUtils_TITLE(VARIANT)
#include    <variant>
#define     QoUtils_VARIANT 1
#else
#define     QoUtils_VARIANT 0
#endif

#undef QoUtils_TITLE
#endif //QoUtils_DEF_GUARD_HPP
