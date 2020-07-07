#include <QtTest>

#include <vector>
#include "map.hpp"

const auto plus1  = [](auto a){ return a + 1; };
const auto minus3 = [](auto a){ return a - 3; };
const auto mul5   = [](auto a){ return a * 5; };
const auto null   = [](auto && a) { a = 0;};

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
    }

};

QTEST_APPLESS_MAIN(test_map)

#include "test_map.moc"
