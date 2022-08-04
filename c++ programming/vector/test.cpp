#include <catch.hpp>

#include <string>
#include <vector>

#include <vector.h>

void Check(const Vector& actual, const std::vector<int>& expected) {
    REQUIRE(actual.Size() == expected.size());
    for (size_t i = 0; i < actual.Size(); ++i) {
        REQUIRE(actual[i] == expected[i]);
    }
}

TEST_CASE("Vector has constructors", "[vector]") {
    {
        Vector a;
        REQUIRE(a.Size() == 0u);
    }
    {
        Vector a{1, 2, 3, 4};
        Check(a, std::vector<int>{1, 2, 3, 4});
    }
    {
        Vector a(5);
        Check(a, std::vector<int>(5));
    }
}

TEST_CASE("Basic methods", "[vector]") {
    Vector a{1, 3, 5};
    REQUIRE(a.Capacity() == 3u);
    Check(a, std::vector<int>{1, 3, 5});

    a.PopBack();
    Check(a, std::vector<int>{1, 3});
    a.Clear();
    REQUIRE(a.Capacity() == 3u);
    Check(a, std::vector<int>());

    a.PushBack(6);
    Check(a, std::vector<int>{6});

    Vector b{3, 4};
    a.Swap(b);
    Check(a, std::vector<int>{3, 4});
    Check(b, std::vector<int>{6});
}

TEST_CASE("Modifications with []", "[vector]") {
    Vector a{3, 7, 8};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    Check(a, std::vector<int>{1, 2, 3});
}

TEST_CASE("Vector iterators 1", "[vector]") {
    Vector a(5);
    for (size_t i = 0; i < a.Size(); ++i) {
        a[i] = i;
    }
    {
        int cur = 0;
        for (int elem : a) {
            REQUIRE(cur++ == elem);
        }
    }

    {
        auto first = a.Begin();
        auto last = a.End();
        REQUIRE(last - first == a.Size());
        REQUIRE(*first == 0);
        ++first;
        REQUIRE(*first == 1);
        auto tmp = first++;
        REQUIRE(*tmp == 1);
        REQUIRE(*first == 2);
        auto end = last--;
        REQUIRE(end == a.End());
        REQUIRE(*last == 4);
        --last;
        REQUIRE((first += 1) == last);
        last -= 3;
        REQUIRE(*last == 0);
    }

    {
        auto first = a.Begin();
        int size = static_cast<int>(a.Size());
        for (int i = 0; i < size; ++i) {
            for (int diff = -2; diff <= 2; ++diff) {
                if (i + diff >= 0 && i + diff < size) {
                    REQUIRE(*(first + i + diff) == i + diff);
                }
            }
        }
    }
}

TEST_CASE("Vector iterators 2", "[vector]") {
    Vector a{1, 3, 5};
    *(a.begin().operator->()) = 2;
    *((--a.end()).operator->()) = 4;

    Check(a, std::vector<int>{2, 3, 4});

    Vector::Iterator it;
    it = a.Begin() + 1;
    REQUIRE(*it == 3);
}

TEST_CASE("Reallocations", "[vector]") {
    const int steps = 16;
    Vector data;
    for (int step = 0; step <= steps; ++step) {
        std::vector<int> ok_data;
        for (int i = 0; i <= (1 << step); ++i) {
            data.PushBack(i);
            ok_data.push_back(i);
        }
        Check(data, ok_data);
        REQUIRE(data.Capacity() == (1u << (step + 1)));
        data.Clear();
    }
}

TEST_CASE("Reserve", "[vector]") {
    Vector a;
    a.Reserve(5);
    REQUIRE(a.Size() == 0u);
    REQUIRE(a.Capacity() == 5u);

    for (int i = 0; i < 5; ++i) {
        a.PushBack(i);
    }

    REQUIRE(a.Capacity() == 5u);
    a.PushBack(4);
    REQUIRE(a.Capacity() == 10u);
    a.Reserve(3);
    REQUIRE(a.Capacity() == 10u);
    a.Clear();
    REQUIRE(a.Size() == 0u);
    REQUIRE(a.Capacity() == 10u);
}

TEST_CASE("Copy correctness", "[vector]") {
    Vector a;
    Vector b(a);
    b.PushBack(1);
    Check(a, std::vector<int>());
    Check(b, std::vector<int>{1});

    b = b;
    Check(b, std::vector<int>{1});
    a = b;
    Check(a, std::vector<int>{1});

    b = std::move(a);
    Check(b, std::vector<int>{1});
    Vector c(std::move(b));
    Check(c, std::vector<int>{1});

    Vector d{3, 4, 5};
    Vector e(d);
    Check(e, std::vector<int>{3, 4, 5});
    d.Swap(c);
    Check(e, std::vector<int>{3, 4, 5});
    Check(d, std::vector<int>{1});
    Check(c, std::vector<int>{3, 4, 5});
}
