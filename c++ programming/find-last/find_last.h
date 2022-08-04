#pragma once

#include <stdexcept>

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& val) {
    auto end = last;
    while (first != last) {
        if (*first == val) {
            end = first;
        }
        ++first;
    }
    return end;
}
