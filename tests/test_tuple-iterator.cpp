
#include "module.hpp"

#include <tuple>
#include "tuple-iterator.hpp"

BOOST_AUTO_TEST_SUITE(test_tuple_iterator);

BOOST_AUTO_TEST_CASE(_begin)
{
    std::tuple t = {1,2,3};
    auto it = QoUtils::begin(t);
}

BOOST_AUTO_TEST_SUITE_END();
