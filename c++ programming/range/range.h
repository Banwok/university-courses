#pragma once

#include <vector>
#include <stdexcept>

std::vector<int> Range(int from, int to, int step = 1) {
    std::vector<int> res;
    int i = 1;
    int iter = from;
    if (step > 0) {
        for (from; iter < to; ++i) {
            res.push_back(iter);
            iter = from + (i * step);
        }
    } else if (step < 0) {
        for (from; iter > to; ++i) {
            res.push_back(iter);
            iter = from + (i * step);
        }
    }
    return res;
}