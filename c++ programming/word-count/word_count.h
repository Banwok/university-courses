#pragma once

#include <string>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include <vector>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    std::vector<std::string> expected;
    std::string str = string;
    std::string tmp;
    while (true) {
        tmp = str.substr(0, str.find(delimiter));
        expected.push_back(tmp);
        if (tmp.size() != str.size()) {
            str = str.substr(tmp.size() + delimiter.size());
        } else {
            break;
        }
    }
    return expected;
}

bool IsSeparat(char c) {
    return std::isspace(c) || std::ispunct(c) || std::isdigit(c);
}

int DifferentWordsCount(const std::string& string) {
    std::string s = string;
    std::unordered_set<std::string> res;

    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::transform(s.begin(), s.end(), s.begin(), [](char c) {
        if (IsSeparat(c)) {
            return ' ';
        }
        return c;
    });
    std::vector<std::string> spl = Split(s);
    for (auto i : spl) {
        if (!i.empty()) {
            res.insert(i);
        }
    }
    return res.size();
}
