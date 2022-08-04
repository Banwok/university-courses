#include <catch.hpp>
#include <util.h>
#include <rotate.h>
#include <vector>
#include <algorithm>

TEST_CASE("Simple") {
    {
        std::vector<int> test{1, 4, 3, 2, 6, 7, 0};
        std::vector<int> expected{3, 2, 6, 7, 0, 1, 4};
        Rotate(&test, 2);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{2, 1, 3, 5};
        std::vector<int> expected{5, 2, 1, 3};
        Rotate(&test, 3);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{1, 2};
        std::vector<int> expected{1, 2};
        Rotate(&test, 0);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{1, 2, 3};
        std::vector<int> expected{1, 2, 3};
        Rotate(&test, 3);
        REQUIRE(expected == test);
    }
}

TEST_CASE("Empty") {
    {
        std::vector<int> test, expected;
        Rotate(&test, 0);
        REQUIRE(expected == test);
    }
    {
        std::vector<int> test{0};
        std::vector<int> expected{0};
        Rotate(&test, 1);
        REQUIRE(expected == test);
    }
}

TEST_CASE("Big") {
    const int size = 1e6;
    int to = 1e9;
    int from = -to;
    int shift = 1e5;
    RandomGenerator rnd(7345422);
    auto test = rnd.GenIntegralVector(size, from, to);
    auto expected = test;
    Rotate(&test, shift);
    std::rotate(expected.begin(), expected.begin() + shift, expected.end());
    REQUIRE(expected == test);
}
