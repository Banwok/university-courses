#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>

class StaticMap {
protected:
    std::vector<std::pair<std::string, std::string>> stmap;

public:
    explicit StaticMap(const std::vector<std::pair<std::string, std::string>>& items) {
        stmap = items;
        std::sort(stmap.begin(), stmap.end(), [](auto a, auto b) { return a.first < b.first; });
    }

    bool Find(const std::string& key, std::string* value) const {
        auto prc_info = std::lower_bound(stmap.begin(), stmap.end(), key,
                                         [](const std::pair<std::string, std::string>& info,
                                            std::string value) { return info.first < value; });
        if (prc_info != stmap.end()) {
            if (stmap[prc_info - stmap.begin()].first != key) {
                return false;
            }
            *value = stmap[prc_info - stmap.begin()].second;
            return true;
        }
        return false;
    }
};
