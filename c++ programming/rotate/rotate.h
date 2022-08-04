#pragma once

#include <stdexcept>
#include <vector>

void Swap(int& a, int& b) {
    a = a ^ b;
    b = b ^ a;
    a = a ^ b;
}

void Rotate(std::vector<int>* data, size_t shift) {
    if ((*data).size() == shift) {
        return;
    }
    auto first = (*data).begin();
    auto middle = (*data).begin() + shift;
    auto next = middle;
    while (first != next) {
        Swap(*first++, *next++);
        if (next == (*data).end()) {
            next = middle;
        } else if (first == middle) {
            middle = next;
        }
    }
}
