#include <catch.hpp>
#include <util.h>
#include <static_map.h>

TEST_CASE("EmptyMap") {
    StaticMap map({});

    std::string value;
    REQUIRE(false == map.Find("key", &value));
}

TEST_CASE("SmallMap") {
    StaticMap map({{"b", "1"}, {"a", "2"}});

    std::string value;
    REQUIRE(true == map.Find("b", &value));
    REQUIRE("1" == value);

    REQUIRE(true == map.Find("a", &value));
    REQUIRE("2" == value);

    REQUIRE(false == map.Find("c", &value));
    REQUIRE(false == map.Find("0", &value));
}

TEST_CASE("Speed") {
    const int num_elements = 100000;

    std::vector<std::pair<std::string, std::string>> items;
    for (int i = 0; i < num_elements; ++i) {
        items.emplace_back(std::to_string(i), std::to_string(i));
    }

    RandomGenerator rnd(73467345);
    rnd.Shuffle(items.begin(), items.end());

    StaticMap map(items);

    for (int i = 0; i < num_elements; ++i) {
        std::string value;
        REQUIRE(true == map.Find(std::to_string(i), &value));
        REQUIRE(std::to_string(i) == value);
    }
}
