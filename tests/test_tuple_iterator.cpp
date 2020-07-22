
#include "module.hpp"

#include <optional>

#include "tuple_iterator.hpp"
#include "container_viewer.hpp"

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
    std::tuple t = {2.3, 4.3, 42U, -4, true, 4,  true, -4.0F, 3, 7, 300.33F, true};

    double d_sum = 0.0;
    double i_sum = 0;

    for(auto it = begin(t); it != end(t); ++it)
    {
        if (auto [v, success] = get_common_of<double, float>(it); success)
        {
            static_assert (std::is_same_v<decltype(v), double>);

            d_sum += v;
            v = 0;
        }
        else if(auto [v, success] = get<int>(it); success)
        {
            static_assert (std::is_same_v<decltype(v), int&>);

            i_sum += v;
            v = -1;
        }
        else if (auto v = get<std::optional<bool>>(it))
        {
            static_assert (std::is_same_v<decltype(v), std::optional<std::reference_wrapper<bool>>>);

            BOOST_CHECK(v.value());
            v.value().get() = false;
        }
        else if (auto [v, success] = get<unsigned>(it); success)
        {
            static_assert (std::is_same_v<decltype(v), unsigned&>);
            v = v * 32U + 1;
        }
    }

    BOOST_CHECK_CLOSE_FRACTION(d_sum, 2.3 + 4.3 -4.0 + 300.33, 0.01);
    BOOST_CHECK_EQUAL(i_sum, -4 + 4 + 3 + 7);

    const std::tuple r {2.3, 4.3, 42U * 32U + 1, -1, false, -1,  false, -4.0F, -1, -1, 300.33F, false};

    BOOST_CHECK_CLOSE_FRACTION(std::get<0>(t), std::get<0>(r), 0.01);
    BOOST_CHECK_CLOSE_FRACTION(std::get<1>(t), std::get<1>(r), 0.01);
    BOOST_CHECK_EQUAL(std::get<2>(t), std::get<2>(r));
    BOOST_CHECK_EQUAL(std::get<3>(t), std::get<3>(r));
    BOOST_CHECK_EQUAL(std::get<4>(t), std::get<4>(r));
    BOOST_CHECK_EQUAL(std::get<5>(t), std::get<5>(r));
    BOOST_CHECK_EQUAL(std::get<6>(t), std::get<6>(r));
    BOOST_CHECK_CLOSE_FRACTION(std::get<7>(t), std::get<7>(r), 0.01);
    BOOST_CHECK_EQUAL(std::get<8>(t), std::get<8>(r));
    BOOST_CHECK_EQUAL(std::get<9>(t), std::get<9>(r));
    BOOST_CHECK_CLOSE_FRACTION(std::get<10>(t), std::get<10>(r), 0.01);
    BOOST_CHECK_EQUAL(std::get<11>(t), std::get<11>(r));
}

BOOST_AUTO_TEST_CASE(test_operator_deref)
{
    using namespace QoUtils;

    std::tuple<const int, double> t {1, 3};

    auto it = begin(t);
    auto a = *it; ++it;
    auto b = *it;

    BOOST_CHECK_EQUAL         (get<const int>(a).first, 1);
    BOOST_CHECK_CLOSE_FRACTION(get<   double>(b).first, 3, 0.001);
}

BOOST_AUTO_TEST_CASE(test_foreach)
{
    using namespace QoUtils;
    std::tuple t = {1, 3, 2.3, 0.03F, true};

    for(auto v : covi(t))
    {
        if (auto [item, success] = get<bool>(v); success)
        {
            BOOST_CHECK_EQUAL(item, true);
        }
        if (auto [item, success] = get<double>(v); success)
        {
            BOOST_CHECK_CLOSE_FRACTION(item, 2.3, 0.001);
        }
        if (auto [item, success] = get<float>(v); success)
        {
            BOOST_CHECK_CLOSE_FRACTION(item, 0.03, 0.001);
        }
        if (auto [item, success] = get<int>(v); success)
        {
            BOOST_CHECK(item == 1 || item == 3);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END();
