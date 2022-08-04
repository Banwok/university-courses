#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

std::unordered_map<int, std::string> ReverseMap(std::unordered_map<std::string, int> map) {
    std::unordered_map<int, std::string> res(map.size());
    for (auto it = map.begin(); it != map.end(); ++it) {
        res[it->second] = it->first;
    }
    return res;
}
