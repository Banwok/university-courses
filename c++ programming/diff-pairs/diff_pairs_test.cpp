#include <catch.hpp>

#include <diff_pairs.h>

TEST_CASE("Simple") {
    {
        std::vector<int> data{2, 3, 4};
        REQUIRE(1 == CountPairs(data, 5));
    }
    {
        std::vector<int> data{2, 2, 2};
        REQUIRE(3 == CountPairs(data, 4));
    }
    {
        std::vector<int> data{2, 2, 2, 1, 3, 1, 3};
        REQUIRE(7 == CountPairs(data, 4));
    }
    {
        std::vector<int> data{};
        REQUIRE(0 == CountPairs(data, 4));
    }
    {
        std::vector<int> data{0, 0, 0, 0};
        REQUIRE(0 == CountPairs(data, 4));
    }
    {
        std::vector<int> data{-1, 1, -1, 1};
        REQUIRE(4 == CountPairs(data, 0));
    }
    {
        std::vector<int> data{656475457, -1000000000, 1000000000};
        REQUIRE(0 == CountPairs(data, -1717507946));
    }
}
