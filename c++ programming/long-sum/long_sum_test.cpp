#include <catch.hpp>

#include <long_sum.h>

#include <algorithm>
#include <vector>

TEST_CASE("Your test") {
    REQUIRE("4" == LongSum("2", "2"));
}

TEST_CASE("simple") {
    { REQUIRE("124" == LongSum("62", "62")); }

    { REQUIRE("1000" == LongSum("1", "999")); }

    { REQUIRE("1000" == LongSum("999", "1")); }

    { REQUIRE("1001" == LongSum("969", "32")); }

    { REQUIRE("1001" == LongSum("32", "969")); }

    {
        REQUIRE("2222222230550369369130809909178" ==
                LongSum("8328447534758347534857", "2222222222221921834372462374321"));
    }

    {
        std::string expect = "2";
        std::string val = "1";
        for (size_t i = 0; i < 1000000 - 1; ++i) {
            expect += "0";
            val += "0";
        }
        REQUIRE(expect == LongSum(val, val));
    }
}