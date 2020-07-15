
#include <vector>
#include <tuple>
#include <optional>

#ifdef QoUtils_OPTIONAL
#error QoUtils_OPTIONAL must be defined after 'def-guard.hpp' included!
#endif

#ifdef QoUtils_VECTOR
#error QoUtils_VECTOR must be defined after 'def-guard.hpp' included!
#endif

#ifdef QoUtils_TUPLE
#error QoUtils_TUPLE must be defined after 'def-guard.hpp' included!
#endif

#include "def_guard.hpp"

#ifndef QoUtils_OPTIONAL
#error QoUtils_OPTIONAL has not been defined!
#endif

#ifndef QoUtils_VECTOR
#error QoUtils_VECTOR has not been defined!
#endif

#ifndef QoUtils_TUPLE
#error QoUtils_TUPLE has not been defined!
#endif

