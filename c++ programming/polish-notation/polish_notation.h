#pragma once

#include <string>
#include <stdexcept>
#include <stack>
#include <vector>
#include <regex>

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

bool IsInteger(const std::string& s) {
    return std::regex_match(s, std::regex("[+-]?[0-9]+"));
}

int EvaluateExpression(const std::string& expression) {
    std::vector<std::string> polish = Split(expression);
    std::stack<int> res;
    int ans;
    for (size_t i = 0; i < polish.size(); ++i) {
        if (IsInteger(polish[i])) {
            int num = std::stoi(polish[i]);
            res.push(num);
        } else {
            int a = res.top();
            res.pop();
            int b = res.top();
            res.pop();
            if (polish[i] == "+") {
                ans = a + b;
            } else if (polish[i] == "-") {
                ans = b - a;
            } else if (polish[i] == "*") {
                ans = a * b;
            } else if (polish[i] == "/") {
                ans = a / b;
            } else {
                throw("error");
            }
            res.push(ans);
        }
    }
    ans = res.top();
    return ans;
}
