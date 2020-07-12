
#include "module.hpp"
#include "tuple-iterator.hpp"

BOOST_AUTO_TEST_SUITE(test_tuple_iterator);

BOOST_AUTO_TEST_CASE(test_begin_and_end)
{
    using namespace QoUtils;

    std::tuple t = {1, 2.2, "3", true, 7U};
    auto it = begin(t);
    {
        auto v = (int*)it;
        BOOST_REQUIRE_NE(v, nullptr);
        BOOST_CHECK_EQUAL(*v, std::get<0>(t));
    }
    ++it;
    {
        auto v = (double*)it;
        BOOST_REQUIRE_NE(v, nullptr);
        BOOST_CHECK_EQUAL(*v, std::get<1>(t));
    }
    it++;
    {
        auto v = (char*)it;
        BOOST_REQUIRE_EQUAL(v, nullptr);
    }
    ++it;
    {
        auto v = (bool*)it;
        BOOST_REQUIRE_NE(v, nullptr);
        BOOST_CHECK_EQUAL(*v, std::get<3>(t));
    }
    it++;
    {
        auto v = (int*)it; // unsigned
        BOOST_REQUIRE_EQUAL(v, nullptr);
    }

    BOOST_CHECK(--end(t) == it--);
}

BOOST_AUTO_TEST_CASE(test_for)
{
    using namespace QoUtils;
    std::tuple t = {2.3, 4.3, -4, 4, false, -4.0, 3, 7};

    double d_sum = 0.0;
    double i_sum = 0;

    for(auto it = begin(t); it != end(t); ++it)
    {
        if (auto v = (double*)it)
        {
            d_sum += *v;
        }
        else if(auto v = (int*)it)
        {
            i_sum += *v;
        }
    }

    BOOST_CHECK_CLOSE_FRACTION(d_sum, 2.3 + 4.3 -4.0, 0.01);
    BOOST_CHECK_EQUAL(i_sum, -4 + 4 + 3 + 7);
}

BOOST_AUTO_TEST_SUITE_END();
