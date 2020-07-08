#include <QtTest>

#include <vector>
#include <string>

#include "map.hpp"

const auto plus1  = [](auto a){ return a + 1; };
const auto minus3 = [](auto a){ return a - 3; };
const auto mul5   = [](auto a){ return a * 5; };

const auto null   = [](auto && a) { a = 0;};

const auto throwIf42         = [](auto && a) { {if (a==42)   throw 42;} return a; };
const auto throwIf42ElseNull = [](auto && a) { {if (a==42)   throw 42;} return a = 0; };
const auto throwIf42ElseMove = [](auto && a) { {if (a=="42") throw 42;} auto b = std::move(a);
                                                                        return b; };

class test_map : public QObject
{
    Q_OBJECT

public:
    test_map() {}
    ~test_map() {}

private slots:
    void test_vectors()
    {
        using namespace QoUtils;

        {
            const std::vector<int> v {1, 20, 36, -4, 5, 0};

            QCOMPARE(map(plus1 , v), (std::vector<int> {1+1, 20+1, 36+1, -4+1, 5+1, 0+1}));
            QCOMPARE(map(minus3, v), (std::vector<int> {1-3, 20-3, 36-3, -4-3, 5-3, 0-3}));
            QCOMPARE(map(mul5  , v), (std::vector<int> {1*5, 20*5, 36*5, -4*5, 5*5, 0*5}));
        }

        {
            std::vector<int> v;
            const std::vector<int> base {1, 20, 36, -4, 5, 0};

            {
                v = base;
                auto& temp = map(plus1, v);
                QCOMPARE(v, (std::vector<int> {1+1, 20+1, 36+1, -4+1, 5+1, 0+1}));
                QCOMPARE(v, temp);
                QCOMPARE(&v, &temp);
            }

            {
                v = base;
                auto& temp = map(minus3, v);
                QCOMPARE(v, (std::vector<int> {1-3, 20-3, 36-3, -4-3, 5-3, 0-3}));
                QCOMPARE(v, temp);
                QCOMPARE(&v, &temp);
            }

            {
                v = base;
                auto& temp = map(mul5, v);
                QCOMPARE(v, (std::vector<int> {1*5, 20*5, 36*5, -4*5, 5*5, 0*5}));
                QCOMPARE(v, temp);
                QCOMPARE(&v, &temp);
            }

            {
                v = base;
                auto& temp = map(null, v);
                QCOMPARE(v, (std::vector<int> {0, 0, 0, 0, 0, 0}));
                QCOMPARE(&v, &temp);
            }
        }

        {
            std::vector<std::string> v = {"Hello", "World", "!"};

            constexpr auto reverse = [](std::string && a)
            {
                std::string result {std::move(a)};
                return std::string { result.rbegin(), result.rend() };
            };

            const std::vector r { reverse("Hello"), reverse("World"), reverse("!")};

            QCOMPARE(map(reverse, std::move(v)), r);
            QCOMPARE(v, (std::vector<std::string>{"", "", ""}));
        }

        {
            const std::vector              v1 = {  1, 42, -5, 4};
                  std::vector              v2 = {  1,  2, 42, 3};
                  std::vector<std::string> v3 = {"1", "2", "3", "42"};

            std::vector<int> r1, r2;
            std::vector<std::string> r3;

            for (int i = 1; i <= 3; ++i)
                try {
                    switch(i) {
                    case 1: r1 = map(throwIf42        ,           v1);  QCOMPARE(1, 2);   break;
                    case 2: r2 = map(throwIf42ElseNull,           v2);  QCOMPARE(1, 2);   break;
                    case 3: r3 = map(throwIf42ElseMove, std::move(v3)); QCOMPARE(1, 2);   break;
                    }
                }
                catch (int e)
                {
                    QCOMPARE(e, 42);
                }
            QCOMPARE(r1, (std::vector<int>{}));
            QCOMPARE(r2, (std::vector<int>{}));
            QCOMPARE(r3, (std::vector<std::string>{}));

            QCOMPARE(v1, (std::vector<int>        {1, 42, -5, 4}));
            QCOMPARE(v2, (std::vector<int>        {0,  0, 42, 3}));
            QCOMPARE(v3, (std::vector<std::string>{"", "", "", "42"}));
        }
    }

};

QTEST_APPLESS_MAIN(test_map)

#include "test_map.moc"
