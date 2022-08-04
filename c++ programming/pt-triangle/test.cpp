#include <catch.hpp>
#include <point_triangle.h>
#include <limits>

constexpr int kMaxInt = 1'000'000'000;

TEST_CASE("Zero") {
    Triangle t({Point({0, 0}), Point({0, 0}), Point({0, 0})});

    Point p({0, 0});
    REQUIRE(!IsPointInTriangle(t, p));
}

TEST_CASE("OneLine") {
    Triangle t({Point({0, 0}), Point({1, 1}), Point({3, 3})});

    Point p1({2, 2});
    Point p2({4, 4});
    REQUIRE(IsPointInTriangle(t, p1));
    REQUIRE(!IsPointInTriangle(t, p2));
}

TEST_CASE("OneLine2") {
    Triangle t({Point({0, 0}), Point({0, 0}), Point({3, 3})});

    Point p1({2, 2});
    Point p2({4, 4});
    REQUIRE(IsPointInTriangle(t, p1));
    REQUIRE(!IsPointInTriangle(t, p2));
}

TEST_CASE("simple") {
    Triangle t({Point({0, 10}), Point({0, 0}), Point({10, 0})});

    Point p1({1, 1});
    Point p2({-1, -1});
    REQUIRE(IsPointInTriangle(t, p1));
    REQUIRE(!IsPointInTriangle(t, p2));
}

TEST_CASE("corner") {
    Triangle t({Point({0, 10}), Point({0, 0}), Point({10, 0})});

    Point p1({0, 0});
    Point p2({0, 10});
    Point p3({10, 0});
    REQUIRE(IsPointInTriangle(t, p1));
    REQUIRE(IsPointInTriangle(t, p2));
    REQUIRE(IsPointInTriangle(t, p3));
}

TEST_CASE("big") {
    Triangle t(
        {Point({kMaxInt, kMaxInt}), Point({kMaxInt, -kMaxInt}), Point({-kMaxInt, -kMaxInt})});

    Point p1({100, 0});
    Point p2({-100, 0});
    Point p3({0, 0});

    Point p4({kMaxInt, 0});
    Point p5({kMaxInt, kMaxInt});
    Point p6({-kMaxInt, -kMaxInt});
    Point p7({kMaxInt, -kMaxInt});
    Point p8({-kMaxInt, kMaxInt});

    REQUIRE(IsPointInTriangle(t, p1));
    REQUIRE(!IsPointInTriangle(t, p2));
    REQUIRE(IsPointInTriangle(t, p3));
    REQUIRE(IsPointInTriangle(t, p4));
    REQUIRE(IsPointInTriangle(t, p5));
    REQUIRE(IsPointInTriangle(t, p6));
    REQUIRE(IsPointInTriangle(t, p7));
    REQUIRE(!IsPointInTriangle(t, p8));
}