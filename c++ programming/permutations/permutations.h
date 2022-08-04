#pragma once

#include <vector>
#include <stdexcept>
#include <vector>

bool NextSet(std::vector<int>& a, int n) {
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) {
        j--;
    }
    if (j == -1) {
        return false;
    }
    int k = n - 1;
    while (a[j] >= a[k]) {
        k--;
    }
    std::swap(a[j], a[k]);
    int l = j + 1, r = n - 1;
    while (l < r) {
        std::swap(a[l++], a[r--]);
    }
    return true;
}

std::vector<std::vector<int>> GeneratePermutations(size_t len) {
    std::vector<std::vector<int>> res;
    std::vector<int> a(len);
    int n = len;
    for (int i = 0; i < len; ++i) {
        a[i] = i;
    }
    res.push_back(a);
    while (NextSet(a, n)) {
        res.push_back(a);
    }
    return res;
}
