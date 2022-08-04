#pragma once

#include <vector>
#include <stdexcept>
#include <unordered_map>

inline int64_t CountPairs(const std::vector<int>& data, int x) {
    std::unordered_map<int, int> map;
    int64_t res = 0;
    if (data.empty()) {
        return res;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        if (map.find(static_cast<int64_t>(x) - static_cast<int64_t>(data[i])) != map.end()) {
            res += map[static_cast<int64_t>(x) - static_cast<int64_t>(data[i])];
        }
        map[data[i]] = map[data[i]] + 1;
    }
    return res;
}
