#include <catch.hpp>

#include <word_count.h>

#include <vector>

TEST_CASE("Small") {
    REQUIRE(3 == DifferentWordsCount("hello world e"));
    REQUIRE(1 == DifferentWordsCount("Test"));
    REQUIRE(2 == DifferentWordsCount("small,test,"));
}

TEST_CASE("Zero") {
    REQUIRE(0 == DifferentWordsCount(""));
    REQUIRE(0 == DifferentWordsCount("    "));
    REQUIRE(0 == DifferentWordsCount(",123."));
    REQUIRE(0 == DifferentWordsCount("!"));
}

TEST_CASE("Register") {
    REQUIRE(4 == DifferentWordsCount("hello Hello WORLD w,orld wOrld"));
    REQUIRE(1 == DifferentWordsCount("a A    a A"));
    REQUIRE(3 == DifferentWordsCount("    register REGISTER matters 'x'"));
}

TEST_CASE("Some tests") {
    REQUIRE(2 == DifferentWordsCount("12shit happens "));
    REQUIRE(5 == DifferentWordsCount(",,abc acb bac bca     cabCba    "));
}
