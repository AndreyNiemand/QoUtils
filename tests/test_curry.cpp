#include "module.hpp"

#include "curry.hpp"

BOOST_AUTO_TEST_SUITE(test_curry);

unsigned f(int x, double y, unsigned z) { return (unsigned)(2*x + y/3 + z/1.2); }

BOOST_AUTO_TEST_CASE(curry_f_value)
{
    using namespace QoUtils;
    const auto f_c = curry(f);

    BOOST_CHECK_EQUAL((f_c( 1)(   2)( 3)), (f( 1,    2,  3)));
    BOOST_CHECK_EQUAL((f_c(-5)(23.4)(53)), (f(-5, 23.4, 53)));
} 

BOOST_AUTO_TEST_SUITE_END();