#pragma once

#include <utility>
#include <vector>
#include <stdexcept>
#include <cmath>

std::vector<std::pair<int64_t, int>> Factorize(int64_t x) {
    std::vector<std::pair<int64_t, int>> delitel;
    for (int64_t i = 2; i <= sqrt(x); ++i) {
        int64_t j = 0;
        while (x % i == 0) {
            delitel.emplace_back(i, ++j);
            if (j > 1) {
                delitel[delitel.size() - 2].second = j;
                delitel.pop_back();
            }
            x /= i;
        }
    }
    if (x != 1) {
        delitel.emplace_back(x, 1);
    }
    return delitel;
}
