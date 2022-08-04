#include <catch.hpp>

#include <quadratic.h>

#include <vector>
#include <cmath>

TEST_CASE("No Roots") {
    REQUIRE(RootCount::kZero == SolveQuadratic(0, 0, 5).count);
    REQUIRE(RootCount::kZero == SolveQuadratic(1, 0, 3).count);
    REQUIRE(RootCount::kZero == SolveQuadratic(4, -4, 7).count);
}

TEST_CASE("One Root") {
    {
        auto result = SolveQuadratic(0, 3, 4);
        REQUIRE(RootCount::kOne == result.count);
        REQUIRE(Approx(-4.0 / 3) == result.first);
    }
    {
        auto result = SolveQuadratic(9, -12, 4);
        REQUIRE(RootCount::kOne == result.count);
        REQUIRE(Approx(2.0 / 3) == result.first);
    }
    {
        auto result = SolveQuadratic(5, 0, 0);
        REQUIRE(RootCount::kOne == result.count);
        REQUIRE(Approx(0.0) == result.first);
    }
}

TEST_CASE("Two Roots") {
    {
        auto result = SolveQuadratic(-5, 19, 12);
        REQUIRE(RootCount::kTwo == result.count);
        REQUIRE(Approx(1.9 - sqrt(601.0) / 10) == result.first);
        REQUIRE(Approx(1.9 + sqrt(601.0) / 10) == result.second);
    }
    {
        auto result = SolveQuadratic(1, -1, -1);
        REQUIRE(RootCount::kTwo == result.count);
        REQUIRE(Approx(0.5 - sqrt(5.0) / 2) == result.first);
        REQUIRE(Approx(0.5 + sqrt(5.0) / 2) == result.second);
    }
}

TEST_CASE("Inf Roots") {
    REQUIRE(RootCount::kInf == SolveQuadratic(0, 0, 0).count);
}
