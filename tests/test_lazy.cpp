#include "module.hpp"
#include "lazy.hpp"

BOOST_AUTO_TEST_SUITE(test_lazy);

BOOST_AUTO_TEST_CASE(simple_test)
{
    using QoUtils::Lazy;
    bool is_calc = false;
    Lazy<int> x = [&is_calc](){ is_calc = true; return 3; };

    BOOST_CHECK(!is_calc);
    BOOST_CHECK_EQUAL(x, 3);
    BOOST_CHECK(is_calc);
}

BOOST_AUTO_TEST_SUITE_END();
