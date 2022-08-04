#include <catch.hpp>
#include <eval.h>

TEST_CASE("Simple") {
    REQUIRE(1 == EvaluateExpression("1"));
    REQUIRE(-2 == EvaluateExpression("-2"));
    REQUIRE(3 == EvaluateExpression("1+2"));
    REQUIRE(6 == EvaluateExpression("2*3"));
    REQUIRE(9 == EvaluateExpression("10-1"));
    REQUIRE(5 == EvaluateExpression("10/2"));
    REQUIRE(8 == EvaluateExpression("-2*-4"));
}

TEST_CASE("Complex") {
    REQUIRE(362944 == EvaluateExpression("2*(1+2*(3*4+105*431)*2+((2*3)*10+343))"));
    REQUIRE(36 == EvaluateExpression("(((1 + 1)*3)*4)+12"));
    REQUIRE(2 == EvaluateExpression("(((1 + 1)))"));
    REQUIRE(30870 == EvaluateExpression("(2+3)*(4+5*1234)"));
    REQUIRE(3628800 == EvaluateExpression("1*2*3*4*5*6*7*8*9* 10"));
    REQUIRE(55 == EvaluateExpression("1+2+3+4+5+6+7+8+9+10"));
    REQUIRE(5 == EvaluateExpression("(((2))+(3))"));
    REQUIRE(
        62557728738473LL ==
        EvaluateExpression("-1- -2-(-(-(0-2-3-4-5+ -2-(3))+1)-3-2     -19) * 17239 * 82473842"));
    REQUIRE(47 == EvaluateExpression("19 + 2 / 3 + (1 + 1 + 394) / 7 / 2 / 1"));
}
