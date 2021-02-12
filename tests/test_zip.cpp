#include "module.hpp"
#include "zip.hpp"

#include <algorithm>
#include <array>

BOOST_AUTO_TEST_SUITE(test_zip);

BOOST_AUTO_TEST_CASE(iterators_general_operations)
{
    using namespace QoUtils;
    auto a1 = std::array {1, 20, 36};
    auto a2 = std::array {2, -3, 43};
 
    auto z = zip(a1, a2);
    {
        auto iter = z.begin();
        BOOST_CHECK(    iter    == z.begin());
        BOOST_CHECK(*  (iter++) == (std::tuple{  1,  2 }));
        BOOST_CHECK(*   iter    == (std::tuple{ 20, -3 }));
        BOOST_CHECK(*(++iter)   == (std::tuple{ 36, 43 }));
        BOOST_CHECK(  ++iter    == z.end());
    }
    {
        auto iter = z.cbegin();
        BOOST_CHECK(    iter    == z.cbegin());
        BOOST_CHECK(*  (iter++) == (std::tuple{  1,  2 }));
        BOOST_CHECK(*   iter    == (std::tuple{ 20, -3 }));
        BOOST_CHECK(*(++iter)   == (std::tuple{ 36, 43 }));
        BOOST_CHECK(  ++iter    == z.cend());
    }

    {
        int sum = 0;
        for (auto [x, y] : zip(a1, a2))
        {
            sum += (x - y);
        }
        BOOST_CHECK_EQUAL(sum, (1-2) + (20-(-3)) + (36-43));
    }
    {
        auto iter = z.begin();
        BOOST_CHECK(    iter    == z.begin());
        *  iter = std::tuple{ 3, 4 };
        *++iter = std::tuple{ 2,-1 };
        *++iter = std::tuple{ 0,-5 };
        BOOST_CHECK(  ++iter    == z.end());
        BOOST_CHECK(a1 == (std::array{3, 2, 0}));
        BOOST_CHECK(a2 == (std::array{4,-1,-5}));
    }

    {
        const auto t1 = a1;
        const auto t2 = a2;
        for (auto& [x,y] : zip(a1, a2))
        {
            std::swap(x,y);
        }

        BOOST_CHECK(a1 == t2);
        BOOST_CHECK(a2 == t1);
    }
}  

BOOST_AUTO_TEST_SUITE_END();