#pragma once
#include <vector>

struct Sum {
    Sum() {
    }
    void operator()(int& a, const int& b) {
        a += b;
    }
};

struct Prod {
    Prod() {
    }
    void operator()(int& a, const int& b) {
        a *= b;
    }
};

struct Concat {
    Concat() {
    }
    void operator()(std::vector<int>& a, const std::vector<int>& b) {
        a.insert(a.end(), b.begin(), b.end());
    }
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    while (first != last) {
        func(init, *first);
        ++first;
    }
    return init;
}

class Length {
protected:
    int* ptr;

public:
    Length(int* ptr_f) {
        ptr = ptr_f;
    }
    template <typename T>
    void operator()(const T& a, const T& b) {
        ++(*ptr);
    }
};
