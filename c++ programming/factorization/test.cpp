#include <catch.hpp>

#include <factorization.h>
#include <vector>

using Factors = std::vector<std::pair<int64_t, int>>;

TEST_CASE("Primes") {
    {
        Factors expected{{2, 1}};
        REQUIRE(expected == Factorize(2));
    }
    {
        Factors expected{{17, 1}};
        REQUIRE(expected == Factorize(17));
    }
    {
        int num = 1000000007;
        Factors expected{{num, 1}};
        REQUIRE(expected == Factorize(num));
    }
    {
        int64_t num = 999999999937LL;
        Factors expected{{num, 1}};
        REQUIRE(expected == Factorize(num));
    }
}

TEST_CASE("Some Tests") {
    {
        Factors expected{{17239, 2}};
        REQUIRE(expected == Factorize(17239ll * 17239));
    }
    {
        Factors expected{{2, 2}, {3, 2}, {7, 2}, {101, 4}};
        REQUIRE(expected == Factorize(183562547364ll));
    }
    {
        Factors expected{{3, 3}, {5, 5}, {13, 3}};
        REQUIRE(expected == Factorize(185371875ll));
    }
    {
        Factors expected{{23, 4}, {100003, 1}};
        REQUIRE(expected == Factorize(27984939523ll));
    }
    {
        Factors expected{{2, 2}, {17, 1}};
        REQUIRE(expected == Factorize(2 * 2 * 17));
    }
}
