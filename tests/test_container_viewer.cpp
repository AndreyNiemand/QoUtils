
#include "module.hpp"

#include <array>
#include <optional>

#include "container_viewer.hpp"

BOOST_AUTO_TEST_SUITE(test_container_viewer);

BOOST_AUTO_TEST_CASE(iterator)
{
    using namespace QoUtils;

    std::array<int, 4> a1 {1,   2,  3,  4};
    std::array<int, 4> a2 {10, 20, 30, 40};

          auto iters       = ContainerViewer(a1, a2).begin();
    const auto iters_end   = ContainerViewer(a1, a2).end  ();

    BOOST_CHECK(std::get<0>(iters) == a1.begin());
    BOOST_CHECK(std::get<1>(iters) == a2.begin());

    BOOST_CHECK(std::get<0>(iters_end) == a1.end());
    BOOST_CHECK(std::get<1>(iters_end) == a2.end());

    auto it1 = a1.begin();
    auto it2 = a2.begin();

    ++it1; it2++;
    ++iters;

    BOOST_CHECK(std::get<0>(iters) == it1);
    BOOST_CHECK(std::get<1>(iters) == it2);

    BOOST_CHECK(std::get<0>(*iters) == *it1);
    BOOST_CHECK(std::get<1>(*iters) == *it2);

    --it1; it2--;
    for (auto [x,y] : ContainerViewer{ a1, a2 })
    {
        BOOST_CHECK(x == *it1);
        BOOST_CHECK(y == *it2);
        ++it1;
        ++it2;
    }
};

BOOST_AUTO_TEST_CASE(value)
{
//    using namespace QoUtils;

//    std::array  v = {+1,+2,+6};
//    std::array  a = {-1,-2,  };

//    for (auto [x,y] : covi(a, v))
//    {
//        BOOST_CHECK(x == -y);
//        BOOST_CHECK(x != 6 && y != 6);
//    }
};

BOOST_AUTO_TEST_CASE(optional)
{
    using namespace QoUtils;

    std::array  v = {+1,+2,+6};
    std::array  a = {-1,-2,  };

//    for (auto [x,y] : covi<std::optional>(a, v))
//    {
//        if (x != std::nullopt && y != std::nullopt)
//            BOOST_CHECK(x == -y.value());
//    }
};

BOOST_AUTO_TEST_SUITE_END();
