#pragma once

#include <string>
#include <stdexcept>

inline std::string LongSum(const std::string& a, const std::string& b) {
    auto it1 = a.end();
    auto it2 = b.end();
    int64_t tmp = 0;
    std::string res;

    int size = std::min(a.size(), b.size());
    int msize = std::max(a.size(), b.size());
    int i = 0;
    for (; i < size; ++i) {
        it1--;
        it2--;
        int cur = (*it1 + *it2 - 2 * '0' + tmp) % 10;
        tmp = (*it1 + *it2 - 2 * '0' + tmp) / 10;
        res += cur + '0';
    }
    auto it = a.size() > b.size() ? it1 : it2;

    for (; i < msize; ++i) {
        it--;
        char cur = (*it - '0' + tmp) % 10;
        tmp = (*it - '0' + tmp) / 10;
        res += cur + '0';
    }

    if (tmp > 0) {
        res += tmp + '0';
    }
    std::reverse(res.begin(), res.end());
    return res;
}