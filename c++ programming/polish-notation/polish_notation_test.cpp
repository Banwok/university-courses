#include <catch.hpp>

#include <polish_notation.h>

#include <string>

TEST_CASE("Short") {
    REQUIRE(1 == EvaluateExpression("1"));
}

TEST_CASE("Some tests") {
    REQUIRE(5 == EvaluateExpression("2 3 +"));
    REQUIRE(6 == EvaluateExpression("2 2 * 2 +"));
    REQUIRE(1 == EvaluateExpression("10 3 3 * -"));
    REQUIRE(0 == EvaluateExpression("-3 11 + -8 + 4 *"));
    REQUIRE(20 == EvaluateExpression("-3 -0 - 3 + 1 - -19 * 1 +"));
    REQUIRE(15 == EvaluateExpression("1 2 3 4 5 + + + +"));
    REQUIRE(-9 == EvaluateExpression("1 -1 2 -2 3 -3 * - + * *"));
}
