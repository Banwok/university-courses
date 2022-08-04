#pragma once

#include <memory>
#include <vector>
#include <string>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<std::unique_ptr<string>> res;
    for (size_t i = 0; i < items.size(); ++i) {
        res.push_back(std::make_unique<string>(*(items[i])));
    }
    return res;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<string>> res;
    res.push_back(std::make_shared<string>(*(items[0])));
    bool fl;
    for (size_t i = 1; i < items.size(); ++i) {
        for (size_t j = 0; j < res.size(); ++j) {
            if (*res[j] == *items[i]) {
                res.push_back(res[j]);
                fl = false;
                break;
            }
            fl = true;
        }
        if (fl) {
            res.push_back(std::make_shared<string>(*(items[i])));
        }
    }
    return res;
}
