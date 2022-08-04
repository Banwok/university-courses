#include <catch.hpp>

#include <permutations.h>

#include <vector>
#include <algorithm>

using Permutations = std::vector<std::vector<int>>;

TEST_CASE("Small") {
    {
        Permutations expected{{0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0}};
        REQUIRE(expected == GeneratePermutations(3));
    }
    {
        Permutations expected{{0}};
        REQUIRE(expected == GeneratePermutations(1));
    }
    {
        Permutations expected{{0, 1}, {1, 0}};
        REQUIRE(expected == GeneratePermutations(2));
    }
}

TEST_CASE("Big") {
    auto result = GeneratePermutations(8);

    std::vector<int> expected(8);
    for (int i = 0; i < 8; ++i) {
        expected[i] = i;
    }

    int i = 0;
    do {
        REQUIRE(expected == result[i]);
        ++i;
    } while (std::next_permutation(expected.begin(), expected.end()));
}
