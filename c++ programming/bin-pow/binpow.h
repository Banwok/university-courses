#pragma once

#include <stdexcept>

int BinPow(int a, int64_t b, int c) {
    if (b == 0) {
        return 1 % c;
    }
    int64_t res = 1;
    int64_t tmp_a = static_cast<int64_t>(a);
    while (b > 0) {
        if (b % 2 == 1) {
            res = (res * tmp_a) % c;
            --b;
        } else {
            tmp_a = (tmp_a * tmp_a) % c;
            b /= 2;
        }
    }
    if (res < 0) {
        res = (res + c) % c;
    }
    return res;
}