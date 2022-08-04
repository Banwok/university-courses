#include <catch.hpp>
#include <util.h>
#include <strict_iterator.h>
#include <find_last.h>

#include <string>
#include <vector>
#include <unordered_map>

struct Int {
    Int() {
    }
    Int(int x) : x(x) {
    }
    int x;

    bool operator==(const Int& r) const {
        return x == r.x;
    }
};

TEST_CASE("Simple") {
    std::vector<std::string> data{"aba", "caba", "aba"};
    auto first = MakeStrict(data.begin(), data.begin(), data.end());
    auto last = MakeStrict(data.begin(), data.end(), data.end());
    auto it = FindLast(first, last, "aba");

    auto expected = MakeStrict(data.begin(), data.begin() + 2, data.end());
    REQUIRE(expected == it);
}

TEST_CASE("Empty") {
    std::vector<std::string> data;
    auto first = MakeStrict(data.begin(), data.begin(), data.end());
    auto last = MakeStrict(data.begin(), data.end(), data.end());
    REQUIRE(first == FindLast(first, last, ""));
}

TEST_CASE("Small") {
    std::vector<int> one{1};
    auto first = MakeStrict(one.begin(), one.begin(), one.end());
    auto last = MakeStrict(one.begin(), one.end(), one.end());

    REQUIRE(first == FindLast(first, last, 1));
    REQUIRE(last == FindLast(first, last, 0));
}

TEST_CASE("EqualOnly") {
    std::vector<Int> elems{Int(1), Int(2), Int(1), Int(3)};
    auto first = MakeStrict(elems.begin(), elems.begin(), elems.end());
    auto last = MakeStrict(elems.begin(), elems.end(), elems.end());

    auto expected = MakeStrict(elems.begin(), elems.begin() + 2, elems.end());
    REQUIRE(expected == FindLast(first, last, Int(1)));
}

TEST_CASE("Long") {
    RandomGenerator rnd(85475);
    const int count = 1e5;
    const int val = 100;
    std::vector<int> elems(count);
    std::unordered_map<int, int> last_positions;
    for (int i = 0; i < count; ++i) {
        elems[i] = rnd.GenInt(-val, val);
        last_positions[elems[i]] = i;
    }
    auto first = MakeStrict(elems.begin(), elems.begin(), elems.end());
    auto last = MakeStrict(elems.begin(), elems.end(), elems.end());

    for (int i = -val; i <= val; ++i) {
        auto it = FindLast(first, last, i);
        auto map_it = last_positions.find(i);
        auto distance = std::distance(first, it);
        REQUIRE(((it == last && map_it == last_positions.end()) || distance == map_it->second));
    }
}
