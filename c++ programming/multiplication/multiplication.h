#pragma once

#include <stdexcept>

int64_t Multiply(int a, int b) {
    auto res = static_cast<int64_t>(a) * static_cast<int64_t>(b);
    return res;
}