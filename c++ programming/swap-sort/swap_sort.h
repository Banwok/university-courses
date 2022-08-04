#pragma once

#include <stdexcept>

void Swap(int* a, int* b) {
    if (*a == *b) {
        return;
    }
    *a = *a ^ *b;
    *b = *b ^ *a;
    *a = *a ^ *b;
}

void Sort3(int* a, int* b, int* c) {
    if (*a > *b) {
        Swap(a, b);
    }
    if (*a > *c) {
        Swap(a, c);
    }
    if (*b > *c) {
        Swap(b, c);
    }
}
