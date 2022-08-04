#pragma once

#include <vector>
#include <stdexcept>

std::vector<int> Unique(const std::vector<int>& data) {
    std::vector<int> res = data;
    std::vector<int>::iterator first = res.begin();
    std::vector<int>::iterator last = res.end();
    int i = 0;
    if (first == last) {
        return res;
    }
    while (++first != last) {
        if (!(res[i] == *first)) {
            res[++i] = *first;
        }
    }
    ++i;
    res.erase(res.begin() + i, last);
    return res;
}
