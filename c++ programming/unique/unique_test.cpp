#include <catch.hpp>

#include <unique.h>

#include <random>
#include <vector>
#include <algorithm>

TEST_CASE("Simple") {
    {
        std::vector<int> test{1, 2, 2, 3, 3, 3};
        std::vector<int> expected{1, 2, 3};
        REQUIRE(expected == Unique(test));
    }
    {
        std::vector<int> test{1, 1, 1, 1};
        std::vector<int> expected{1};
        REQUIRE(expected == Unique(test));
    }
    {
        std::vector<int> test{1, 2, 3, 4};
        std::vector<int> expected{1, 2, 3, 4};
        REQUIRE(expected == Unique(test));
    }
    {
        std::vector<int> test{1, 1, 2, 2, 2, 3};
        std::vector<int> expected{1, 2, 3};
        REQUIRE(expected == Unique(test));
    }
}

TEST_CASE("Empty") {
    {
        std::vector<int> test, expected;
        REQUIRE(expected == Unique(test));
    }
    {
        std::vector<int> test{0};
        std::vector<int> expected{0};
        REQUIRE(expected == Unique(test));
    }
}

TEST_CASE("Big") {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random_element(-100, 100);

    std::vector<int> input;
    for (int i = 0; i < 128 * 1024; i++) {
        input.push_back(random_element(generator));
    }

    std::sort(input.begin(), input.end());

    auto expected = input;
    expected.erase(std::unique(expected.begin(), expected.end()), expected.end());

    REQUIRE(expected == Unique(input));
}
