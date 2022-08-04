#include <catch.hpp>
#include <swap_sort.h>
#include <limits>
#include <vector>
#include <algorithm>

TEST_CASE("SwapTest") {
    {
        int a = 5, b = 7;
        Swap(&a, &b);
        REQUIRE(7 == a);
        REQUIRE(5 == b);
    }
    {
        int a = std::numeric_limits<int>::min();
        int b = std::numeric_limits<int>::max();
        Swap(&a, &b);
        REQUIRE(a == std::numeric_limits<int>::max());
        REQUIRE(b == std::numeric_limits<int>::min());
    }
    {
        int a = 1, b = 1;
        Swap(&a, &b);
        REQUIRE(1 == a);
        REQUIRE(1 == b);
    }
}

TEST_CASE("Sort All") {
    std::vector<int> p{1, 2, 3};
    do {
        int a = p[0], b = p[1], c = p[2];
        Sort3(&a, &b, &c);
        REQUIRE(1 == a);
        REQUIRE(2 == b);
        REQUIRE(3 == c);
    } while (std::next_permutation(p.begin(), p.end()));
}

TEST_CASE("Sort Eq") {
    {
        int a = 1, b = 1, c = 1;
        Sort3(&a, &b, &c);
        REQUIRE(1 == a);
        REQUIRE(1 == b);
        REQUIRE(1 == c);
    }
    {
        int a = 2, b = 4, c = 2;
        Sort3(&a, &b, &c);
        REQUIRE(2 == a);
        REQUIRE(2 == b);
        REQUIRE(4 == c);
    }
    {
        int a = 3, b = 3, c = 1;
        Sort3(&a, &b, &c);
        REQUIRE(1 == a);
        REQUIRE(3 == b);
        REQUIRE(3 == c);
    }
}
