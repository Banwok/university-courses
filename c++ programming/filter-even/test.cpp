#include <catch.hpp>
#include <util.h>
#include <filter_even.h>
#include <vector>

TEST_CASE("Simple") {
    {
        std::vector<int> test{1, 4, 3, 2, 6, 7, 0};
        std::vector<int> expected{4, 2, 6, 0};
        FilterEven(&test);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{2, 1, 3, 5};
        std::vector<int> expected{2};
        FilterEven(&test);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{7, 3, 5, 4, 6};
        std::vector<int> expected{4, 6};
        FilterEven(&test);
        REQUIRE(expected == test);
    }
}

TEST_CASE("Empty") {
    {
        std::vector<int> test, expected;
        FilterEven(&test);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{1, -5, -3, 3, 7};
        std::vector<int> expected;
        FilterEven(&test);
        REQUIRE(expected == test);
    }
}

TEST_CASE("Big") {
    const int size = 1e6;
    RandomGenerator rnd(3784657);
    auto test = rnd.GenPermutation(size);
    auto expected = test;
    FilterEven(&test);
    expected.erase(
        std::remove_if(expected.begin(), expected.end(), [](int x) { return x % 2 != 0; }),
        expected.end());
    REQUIRE(expected == test);
}
