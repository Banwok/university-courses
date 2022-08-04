#pragma once

#include <cstdint>
#include <vector>

class Stack {
public:
    void Push(int x) {
        stack.push_back(x);
    }

    bool Pop() {
        if (!stack.empty()) {
            stack.pop_back();
            return true;
        }
        return false;
    }

    int Top() const {
        return stack.back();
    }

    bool Empty() const {
        if (stack.empty()) {
            return true;
        }
        return false;
    }

    size_t Size() const {
        return stack.size();
    }

protected:
    std::vector<int> stack;
};
