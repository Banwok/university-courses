#pragma once

#include <stdexcept>
#include <utility>

template <class Iterator, class Predicate>
Iterator Partition(Iterator first, Iterator last, Predicate pred) {
    while (first != last) {
        --last;
        if (first == last) {
            if (!pred(*first)) {
                return first;
            }
            last++;
            return last;
        }
        if (pred(*first)) {
            ++first;
            if (first == last) {
                if (pred(*first)) {
                    ++first;
                    return first;
                }
                return first;
            }
        }
        if (pred(*last)) {
            while (pred(*first)) {
                ++first;
            }
            std::swap(*first, *last);
            ++first;
            if (first == last) {
                if (pred(*first)) {
                    ++first;
                    return first;
                }
                return first;
            }
        }
    }
    return last;
}
