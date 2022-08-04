#include <catch.hpp>
#include <any.h>
#include <string>
#include <typeinfo>

struct SomeStruct {
    int x;
};

TEST_CASE("Simple") {
    Any a(5);
    REQUIRE(5 == a.GetValue<int>());

    Any b(std::string("abacaba"));
    REQUIRE("abacaba" == b.GetValue<std::string>());

    Any c;
    c = 7.0;
    REQUIRE(false == c.Empty());
    REQUIRE(7.0 == c.GetValue<double>());

    Any d;
    int *ptr = nullptr;
    d = ptr;
    REQUIRE(false == d.Empty());
}

TEST_CASE("Empty") {
    Any a;
    REQUIRE(true == a.Empty());

    std::vector<int> t{1, 2};
    Any b(t);
    REQUIRE(false == b.Empty());
    a.Swap(b);

    REQUIRE(t == a.GetValue<std::vector<int>>());
    REQUIRE(false == a.Empty());
    REQUIRE(true == b.Empty());

    a.Clear();
    REQUIRE(true == a.Empty());
}

TEST_CASE("Copy") {
    Any a(5);
    Any b = a;

    REQUIRE(a.GetValue<int>() == b.GetValue<int>());

    Any c;
    c = b;

    REQUIRE(b.GetValue<int>() == c.GetValue<int>());
    b.Clear();
    REQUIRE(5 == c.GetValue<int>());

    Any d(SomeStruct{3});
    REQUIRE(3 == d.GetValue<SomeStruct>().x);

    d = std::string("check");
    REQUIRE("check" == d.GetValue<std::string>());

    Any e = Any(std::string("dorou"));
    e = e;

    REQUIRE("dorou" == e.GetValue<std::string>());

    a.Swap(e);
    REQUIRE(5 == e.GetValue<int>());
}

TEST_CASE("Any throws") {
    Any a(std::string("just a test"));
    REQUIRE_THROWS_AS(a.GetValue<int>(), std::bad_cast);
    Any b(std::vector<int>{1, 2, 3});
    REQUIRE(b.GetValue<std::vector<int>>().size() == 3);
    REQUIRE_THROWS_AS(b.GetValue<std::string>(), std::bad_cast);
}
