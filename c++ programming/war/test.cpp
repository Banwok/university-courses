#include <catch.hpp>
#include <war.h>
#include <array>
#include <algorithm>
#include <vector>

using Deck = std::array<int, 5>;

TEST_CASE("Simple") {
    {
        Deck f{1, 3, 5, 7, 9};
        Deck s{2, 4, 6, 8, 0};
        auto result = SimulateWarGame(f, s);
        REQUIRE(kSecond == result.winner);
        REQUIRE(5 == result.turn);
    }
}

TEST_CASE("SomeTests") {
    {
        Deck f{1, 2, 3, 4, 0};
        Deck s{5, 6, 7, 8, 9};
        auto result = SimulateWarGame(f, s);
        REQUIRE(kSecond == result.winner);
        REQUIRE(17 == result.turn);
    }
    {
        Deck f{0, 8, 4, 5, 7};
        Deck s{9, 1, 2, 6, 3};
        auto result = SimulateWarGame(f, s);
        REQUIRE(kFirst == result.winner);
        REQUIRE(15 == result.turn);
    }
}

TEST_CASE("Big") {
    Deck f{0, 1, 2, 3, 6};
    Deck s{4, 7, 8, 9, 5};
    auto result = SimulateWarGame(f, s);
    REQUIRE(kFirst == result.winner);
    REQUIRE(327 == result.turn);
}
