#pragma once

#include <string>
#include <stdexcept>
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
