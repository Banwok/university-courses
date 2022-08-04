#include <catch.hpp>
#include <util.h>
#include <strict_iterator.h>
#include <partition.h>

#include <string>
#include <vector>
#include <algorithm>

template <class T, class Iterator, class Predicate>
void CheckPartition(std::vector<T> data, Iterator first, Iterator last, Iterator it,
                    Predicate pred) {
    std::vector<T> result;
    while (first != it) {
        result.push_back(*first);
        REQUIRE(pred(*first));
        ++first;
    }
    while (first != last) {
        result.push_back(*first);
        REQUIRE(!pred(*first));
        ++first;
    }

    std::sort(result.begin(), result.end());
    std::sort(data.begin(), data.end());
    REQUIRE(data == result);
}

TEST_CASE("Small") {
    std::vector<int> data{2, 1, 2, 1};
    auto first = MakeStrict(data.begin(), data.begin(), data.end());
    auto last = MakeStrict(data.begin(), data.end(), data.end());

    auto predicate = [](int x) { return x == 2; };
    auto it = Partition(first, last, predicate);
    CheckPartition(data, first, last, it, predicate);
}

TEST_CASE("Simple") {
    std::vector<int> data{2, 1, 2, 2, 3, 5, 4};
    auto first = MakeStrict(data.begin(), data.begin(), data.end());
    auto last = MakeStrict(data.begin(), data.end(), data.end());

    auto predicate = [](int x) { return x % 2 == 0; };
    auto it = Partition(first, last, predicate);
    CheckPartition(data, first, last, it, predicate);
}

TEST_CASE("Empty") {
    {
        std::vector<int> data;
        auto first = MakeStrict(data.begin(), data.begin(), data.end());
        auto last = MakeStrict(data.begin(), data.end(), data.end());

        auto expected = first;
        REQUIRE(expected == Partition(first, last, [](int) { return false; }));
    }
    {
        std::vector<std::string> data{"aba"};
        auto first = MakeStrict(data.begin(), data.begin(), data.end());
        auto last = MakeStrict(data.begin(), data.end(), data.end());

        auto predicate = [](const std::string& s) { return s.size() < 3; };
        auto it = Partition(first, last, predicate);
        CheckPartition(data, first, last, it, predicate);
    }
    {
        std::vector<std::string> data{"aba"};
        auto first = MakeStrict(data.begin(), data.begin(), data.end());
        auto last = MakeStrict(data.begin(), data.end(), data.end());

        auto predicate = [](const std::string& s) { return s.size() <= 3; };
        auto it = Partition(first, last, predicate);
        CheckPartition(data, first, last, it, predicate);
    }
}

TEST_CASE("Big") {
    RandomGenerator rnd(736475);
    const int count = 1e5;
    const int val = 1e9;
    auto data = rnd.GenIntegralVector(count, -val, val);

    auto first = MakeStrict(data.begin(), data.begin(), data.end());
    auto last = MakeStrict(data.begin(), data.end(), data.end());
    auto predicate = [](int x) { return x < 0; };

    auto it = Partition(first, last, predicate);
    CheckPartition(data, first, last, it, predicate);
}
