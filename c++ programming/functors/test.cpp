#include <catch.hpp>
#include <util.h>
#include <functors.h>

#include <algorithm>
#include <vector>

TEST_CASE("Binary") {
    std::vector<int> a{1, 3, 4, 0, -1, 6};
    auto less = [](int x, int y) { return x < y; };
    std::sort(a.begin(), a.end(), MakeReverseBinaryFunctor(less));

    std::vector<int> expected{6, 4, 3, 1, 0, -1};
    REQUIRE(expected == a);
}

TEST_CASE("String") {
    std::vector<std::string> a{"aba", "caba"};
    auto greater = [](const std::string& a, const std::string& b) { return a > b; };
    std::sort(a.begin(), a.end(), MakeReverseBinaryFunctor(greater));

    std::vector<std::string> expected{"aba", "caba"};
    REQUIRE(expected == a);
}

static bool IsEven(int x) {
    return x % 2 == 0;
}

TEST_CASE("Unary") {
    std::vector<int> a{1, 2, 0, -1, 3, 4};
    auto it = std::remove_if(a.begin(), a.end(), MakeReverseUnaryFunctor(IsEven));
    a.resize(std::distance(a.begin(), it));

    std::vector<int> expected{2, 0, 4};
    REQUIRE(expected == a);
}

TEST_CASE("ComparisonsCount Int") {
    std::vector<int> a{1, 3};
    auto res = ComparisonsCount(a.begin(), a.end());

    REQUIRE(res == 1);
}

TEST_CASE("ComparisonsCount Strings") {
    std::vector<std::string> a{"aba", "caba"};
    auto res = ComparisonsCount(a.begin(), a.end());

    REQUIRE(res == 1);
}

TEST_CASE("ComparisonsCount Pointers") {
    auto a = std::make_shared<int>(1);
    auto b = std::make_shared<int>(2);

    std::vector<std::shared_ptr<int>> c = {a, b};
    auto res = ComparisonsCount(c.begin(), c.end());

    REQUIRE(res == 1);
}
