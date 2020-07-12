
#include "module.hpp"

#include <vector>
#include <string>
#include <tuple>

#include "map.hpp"

BOOST_AUTO_TEST_SUITE(test_map);

const auto plus1  = [](auto a){ return a + 1; };
const auto minus3 = [](auto a){ return a - 3; };
const auto mul5   = [](auto a){ return a * 5; };

const auto null   = [](auto && a) { a = 0;};

const auto throwIf42         = [](auto && a) { {if (a==42)   throw 42;} return a; };
const auto throwIf42ElseNull = [](auto && a) { {if (a==42)   throw 42;} return a = 0; };
const auto throwIf42ElseMove = [](auto && a) { {if (a=="42") throw 42;} auto b = std::move(a);
                                                                        return b; };
BOOST_AUTO_TEST_CASE(vector_clvalue)
{
    using namespace QoUtils;
    const std::vector<int> v {1, 20, 36, -4, 5, 0};

    BOOST_CHECK(map(plus1 , v) == (std::vector<int> {1+1, 20+1, 36+1, -4+1, 5+1, 0+1}));
    BOOST_CHECK(map(minus3, v) == (std::vector<int> {1-3, 20-3, 36-3, -4-3, 5-3, 0-3}));
    BOOST_CHECK(map(mul5  , v) == (std::vector<int> {1*5, 20*5, 36*5, -4*5, 5*5, 0*5}));
}

BOOST_AUTO_TEST_CASE(vector_lvalue)
{
    using namespace QoUtils;
    std::vector<int> v;
    const std::vector<int> base {1, 20, 36, -4, 5, 0};
    {
        v = base;
        auto& temp = map(plus1, v);
        BOOST_CHECK(v == (std::vector<int> {1+1, 20+1, 36+1, -4+1, 5+1, 0+1}));
        BOOST_CHECK(v == temp);
        BOOST_CHECK(&v == &temp);
    }

    {
        v = base;
        auto& temp = map(minus3, v);
        BOOST_CHECK(v == (std::vector<int> {1-3, 20-3, 36-3, -4-3, 5-3, 0-3}));
        BOOST_CHECK(v == temp);
        BOOST_CHECK(&v == &temp);
    }

    {
        v = base;
        auto& temp = map(mul5, v);
        BOOST_CHECK(v == (std::vector<int> {1*5, 20*5, 36*5, -4*5, 5*5, 0*5}));
        BOOST_CHECK(v == temp);
        BOOST_CHECK(&v == &temp);
    }

    {
        v = base;
        auto& temp = map(null, v);
        BOOST_CHECK(v == (std::vector<int> {0, 0, 0, 0, 0, 0}));
        BOOST_CHECK(&v == &temp);
    }
}
BOOST_AUTO_TEST_CASE(vector_rvalue)
{
    using namespace QoUtils;
    std::vector<std::string> v = {"Hello", "World", "!"};

    constexpr auto reverse = [](std::string && a)
    {
        std::string result {std::move(a)};
        return std::string { result.rbegin(), result.rend() };
    };

    const std::vector r { reverse("Hello"), reverse("World"), reverse("!")};

    BOOST_CHECK(map(reverse, std::move(v)) == r);
    BOOST_CHECK(v == (std::vector<std::string>{"", "", ""}));
}
BOOST_AUTO_TEST_CASE(vector_throw_exceptions)
{
    using namespace QoUtils;
    const std::vector              v1 = {  1, 42, -5, 4};
          std::vector              v2 = {  1,  2, 42, 3};
          std::vector<std::string> v3 = {"1", "2", "3", "42"};

    std::vector<int> r1, r2;
    std::vector<std::string> r3;

    for (int i = 1; i <= 3; ++i)
        try {
            switch(i) {
            case 1: r1 = map(throwIf42        ,           v1);  BOOST_CHECK(1 == 2);   break;
            case 2: r2 = map(throwIf42ElseNull,           v2);  BOOST_CHECK(1 == 2);   break;
            case 3: r3 = map(throwIf42ElseMove, std::move(v3)); BOOST_CHECK(1 == 2);   break;
            }
        }
        catch (int e)
        {
            BOOST_CHECK(e == 42);
        }
    BOOST_CHECK(r1 == (std::vector<int>{}));
    BOOST_CHECK(r2 == (std::vector<int>{}));
    BOOST_CHECK(r3 == (std::vector<std::string>{}));

    BOOST_CHECK(v1 == (std::vector<int>        {1, 42, -5, 4}));
    BOOST_CHECK(v2 == (std::vector<int>        {0,  0, 42, 3}));
    BOOST_CHECK(v3 == (std::vector<std::string>{"", "", "", "42"}));
}

BOOST_AUTO_TEST_CASE(tuple_clvalue)
{
    using namespace QoUtils;
    using std::operator ""s;

    const std::tuple t {false, 3, "Hello"};
    const std::tuple r { true, 4, "ello"s};

    BOOST_CHECK(map([](const auto& a){ return a + 1; }, t) == r);
}

BOOST_AUTO_TEST_CASE(tuple_lvalue)
{
    using namespace QoUtils;
    using std::operator ""s;

    std::tuple t = {1,2,3};

    BOOST_CHECK(map([](auto a){return a +  1;}, t) == (std::tuple{ 2, 3, 4}));
    BOOST_CHECK(t == (std::tuple{ 1,2,3 }));
    BOOST_CHECK(map([](auto a){return a * -4;}, t) == (std::tuple{-4,-8,-12}));
    BOOST_CHECK(t == (std::tuple{ 1,2,3 }));

    BOOST_CHECK(map([](auto& a){ a += 2; return a + 1;}, t) == (std::tuple{ 4, 5, 6}));
    BOOST_CHECK(t == (std::tuple{ 3,4,5 }));
    BOOST_CHECK(map([](auto& a){ a =  0; return a - 1;}, t) == (std::tuple{-1,-1,-1}));
    BOOST_CHECK(t == (std::tuple{ 0,0,0 }));
}

BOOST_AUTO_TEST_CASE(tuple_rvalue)
{
    using namespace QoUtils;
    using std::operator ""s;

    const char* World = "World";
    std::tuple t1 { "Hello"s, World, "!"s };
    std::tuple t2 = map([](auto && a){ return std::move(a); }, t1);

    BOOST_CHECK(t1 == (std::tuple{""s, World, ""s}));
    BOOST_CHECK(t2 == (std::tuple{"Hello"s, World, "!"s}));
    BOOST_CHECK(std::get<1>(t1) == std::get<1>(t2));
}

BOOST_AUTO_TEST_CASE(tuple_returning_lvalue)
{
    using namespace QoUtils;

    std::tuple<int, const int, int, int>
            t1 {1,2,3,4};
    std::tuple
            t2 = map([](auto && a) -> auto && { return a;}, t1);

    std::get<0>(t2) = 2;
    std::get<2>(t2) = 20;
    std::get<3>(t2) = -4;

    BOOST_CHECK(t1 == (std::tuple{2, 2, 20, -4}));
    BOOST_CHECK(t2 == (std::tuple{2, 2, 20, -4}));

}

BOOST_AUTO_TEST_CASE(tuple_returning_void)
{
    using namespace QoUtils;

    std::tuple t = {1,2,"3","4"};
    auto f = [](auto a)
    {
        if constexpr (std::is_integral_v<decltype(a)>)
            return a;
    };
    auto d = map(f, t);

    BOOST_CHECK(map(f, t) == (std::tuple{1,2}));
}

BOOST_AUTO_TEST_SUITE_END();
