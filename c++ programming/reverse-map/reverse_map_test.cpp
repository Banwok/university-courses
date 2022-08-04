#include <catch.hpp>

#include <reverse_map.h>

#include <string>
#include <unordered_map>

using Map = std::unordered_map<std::string, int>;
using ReversedMap = std::unordered_map<int, std::string>;

TEST_CASE("Simple") {
    {
        Map test{{"aba", 3}, {"caba", 1}, {"test", 2}};
        ReversedMap expected{{1, "caba"}, {2, "test"}, {3, "aba"}};
        REQUIRE(expected == ReverseMap(test));
    }
    {
        Map test{{"", 0}, {"1", 1}};
        ReversedMap expected{{0, ""}, {1, "1"}};
        REQUIRE(expected == ReverseMap(test));
    }
}

TEST_CASE("Empty") {
    Map test;
    ReversedMap expected;
    REQUIRE(expected == ReverseMap(test));
}
