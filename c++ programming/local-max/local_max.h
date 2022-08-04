#pragma once

#include <stdexcept>

template <class Iterator>
Iterator LocalMax(Iterator first, Iterator last) {
    if (first == last) {
        return last;
    }
    auto tmp = first;
    if (++first == last) {
        return tmp;
    }
    if (*first < *tmp) {
        return tmp;
    }
    auto end = last--;
    auto i = first++;
    if (first == end) {
        if (i == tmp) {
            return end;
        }
        return i;
    }
    for (; first != last; ++i, ++first, ++tmp) {
        if (*tmp < *i && *first < *i) {
            return i;
        }
    }
    first--;
    if (*first < *(last)) {
        return (last);
    }
    return end;
}
