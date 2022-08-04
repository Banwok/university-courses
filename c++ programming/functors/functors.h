#pragma once

#include <vector>

template <class Functor>
class ReverseBinaryFunctor {
protected:
    Functor* functor;

public:
    ReverseBinaryFunctor(Functor& func) {
        functor = &func;
    }
    template <class T1, class T2>
    bool operator()(T1& t1, T2& t2) {
        return (*functor)(t2, t1);
    }
};

template <class Functor>
class ReverseUnaryFunctor {
protected:
    Functor* functor;

public:
    ReverseUnaryFunctor(Functor& func) {
        functor = &func;
    }
    bool operator()(int& arg) {
        return !((*functor)(arg));
    }
};

template <class Functor>
ReverseUnaryFunctor<Functor> MakeReverseUnaryFunctor(Functor& functor) {
    return ReverseUnaryFunctor<Functor>(functor);
}

template <class Functor>
ReverseBinaryFunctor<Functor> MakeReverseBinaryFunctor(Functor& functor) {
    return ReverseBinaryFunctor<Functor>(functor);
}

class CountComparator {
protected:
    int* c;

public:
    CountComparator(int* count) : c(count) {
    }
    template <typename T>
    bool operator()(T& t1, T& t2) {
        ++(*c);
        return t1 < t2;
    }
};

template <class Iterator>
int ComparisonsCount(Iterator first, Iterator last) {
    int count = 0;
    CountComparator comp(&count);
    std::sort(first, last, comp);
    return count;
}