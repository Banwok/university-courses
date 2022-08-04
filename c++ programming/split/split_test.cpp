#include <catch.hpp>

#include <split.h>

#include <string>
#include <vector>

TEST_CASE("Simple") {
    {
        std::vector<std::string> expected{"aba", "caba", "1"};
        REQUIRE(expected == Split("aba caba 1"));
    }
    {
        std::vector<std::string> expected{"aba"};
        REQUIRE(expected == Split("aba"));
    }
}

TEST_CASE("Empty") {
    std::vector<std::string> expected{""};
    REQUIRE(expected == Split(""));
}

TEST_CASE("Full match") {
    std::vector<std::string> expected{"", ""};
    REQUIRE(expected == Split("full match", "full match"));
}

TEST_CASE("Some tests") {
    {
        std::vector<std::string> expected{"just", "", "a", "test", ""};
        REQUIRE(expected == Split("just  a test "));
    }
    {
        std::vector<std::string> expected{"hello", "world,no split here", "", "1", ""};
        REQUIRE(expected == Split("hello, world,no split here, , 1, ", ", "));
    }
    {
        std::vector<std::string> expected{"", "a", "b c", "def", "g h "};
        REQUIRE(expected == Split("  a  b c  def  g h ", "  "));
    }
}
