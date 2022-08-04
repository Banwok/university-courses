#include <catch.hpp>

#include <smart_ptr.h>

#include <string>

using std::string;

TEST_CASE("Empty shared") {
    SharedPtr a;
    SharedPtr b;
    b = a;
    SharedPtr c(a);
    b = std::move(c);
    REQUIRE(a.Get() == nullptr);
    REQUIRE(b.Get() == nullptr);
}

TEST_CASE("Reset shared") {
    string* ptr1 = new string("aba");
    string* ptr2 = new string("caba");
    SharedPtr a(ptr1);
    SharedPtr b(ptr2);
    REQUIRE(a.Get() == ptr1);
    REQUIRE(*b == *ptr2);
    a.Reset(nullptr);
    b = a;
}

TEST_CASE("Copy/move shared") {
    SharedPtr a(new string("aba"));
    string* ptr;
    {
        SharedPtr b(a);
        SharedPtr c(a);
        ptr = c.Get();
    }
    REQUIRE(ptr == a.Get());
    REQUIRE(*ptr == "aba");

    SharedPtr b(new string("caba"));
    {
        SharedPtr c(b);
        SharedPtr d(b);
        d = std::move(a);
        REQUIRE(*c == "caba");
        REQUIRE(*d == "aba");
        b.Reset(new string("test"));
        REQUIRE(*c == "caba");
    }
    REQUIRE(*b == "test");

    SharedPtr end;
    {
        SharedPtr d(new string("delete"));
        d = b;
        SharedPtr c(std::move(b));
        REQUIRE(*d == "test");
        REQUIRE(*c == "test");
        d = d;
        c = end;
        d.Reset(new string("delete"));
        end = d;
    }

    REQUIRE(*end == "delete");
}

TEST_CASE("Empty weak") {
    WeakPtr a;
    WeakPtr b;
    a = b;
    WeakPtr c(a);
    b = std::move(c);

    auto shared = b.Lock();
    REQUIRE(shared.Get() == nullptr);
}

TEST_CASE("Weak expiration") {
    WeakPtr* a;
    {
        SharedPtr b(new string("aba"));
        SharedPtr c(b);
        a = new WeakPtr(c);
        auto test = a->Lock();
        REQUIRE(*test == "aba");
        REQUIRE(!a->IsExpired());
    }
    REQUIRE(a->IsExpired());
    delete a;
}

TEST_CASE("Weak extends shared") {
    SharedPtr* b = new SharedPtr(new string("aba"));
    WeakPtr c(*b);
    auto a = c.Lock();
    delete b;
    REQUIRE(!c.IsExpired());
    REQUIRE(*a == "aba");
}

TEST_CASE("Copy/move weak") {
    SharedPtr a(new string("aba"));
    WeakPtr b(a);
    WeakPtr empty;
    WeakPtr c(b);
    WeakPtr d(a);
    REQUIRE(!c.IsExpired());
    c = empty;
    REQUIRE(c.IsExpired());

    b = std::move(c);
    auto locked = d.Lock();
    REQUIRE(*locked == "aba");

    WeakPtr start(a);
    {
        SharedPtr a2(a);
        WeakPtr f(a2);
        auto cur_lock = f.Lock();
        REQUIRE(cur_lock.Get() == SharedPtr(start).Get());
    }
}

TEST_CASE("Shared from weak") {
    SharedPtr* x = new SharedPtr(new string("aba"));
    WeakPtr y(*x);
    delete x;
    REQUIRE(y.IsExpired());
    SharedPtr z(y);
    REQUIRE(z.Get() == nullptr);
}
