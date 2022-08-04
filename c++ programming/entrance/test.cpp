#include <catch.hpp>
#include <entrance.h>

TEST_CASE("Simple test") {

    std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>> sts = {
        {StudentName({"b", "basd"}), Date({0, 0, 0}), 10, {"mgu"}},
        {StudentName({"b", "aa"}), Date({0, 0, 0}), 10, {"itmo"}},
        {StudentName({"b", "asd"}), Date({0, 0, 0}), 9, {"mgu", "spbu"}},
        {StudentName({"", "bba"}), Date({1, 0, 0}), 8, {"itmo", "spbu"}},
        {StudentName({"", "bbb"}), Date({0, 0, 0}), 8, {"itmo", "spbu"}},
        {StudentName({"a", "aa"}), Date({0, 0, 0}), 10, {"itmo"}},
        {StudentName({"", "ab"}), Date({0, 0, 0}), 9, {"spbu"}},
    };

    std::vector<std::pair<std::string, int>> unis = {
        {"itmo", 1}, {"spbu", 2}, {"mgu", 10}, {"poly", 100}};

    auto res = GetStudents(unis, sts);
    REQUIRE(res["itmo"].size() == 1);
    REQUIRE(res["spbu"].size() == 2);
    REQUIRE(res["mgu"].size() == 2);
    REQUIRE(res["itmo"][0].name == "a");
    REQUIRE(res["spbu"][0].surname == "ab");
    REQUIRE(res["spbu"][1].surname == "bbb");
    REQUIRE(res["mgu"][0].surname == "asd");
    REQUIRE(res["mgu"][1].surname == "basd");
    REQUIRE(res.find("poly") != res.end());
    REQUIRE(res["poly"].empty());
}
