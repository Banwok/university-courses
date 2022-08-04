#pragma once

#include <stdexcept>
#include <cmath>

enum class RootCount { kZero, kOne, kTwo, kInf };

struct Roots {
    RootCount count;
    double first, second;
};

Roots SolveQuadratic(int a, int b, int c) {
    double x1, x2;
    Roots res;
    double a_doub = static_cast<double>(a);
    double b_doub = static_cast<double>(b);
    double c_doub = static_cast<double>(c);
    if (a == b && b == c && c == 0) {
        res.count = RootCount::kInf;
        return res;
    }
    double d = b_doub * b_doub - 4 * a_doub * c_doub;
    if (d == 0) {
        if (a_doub == 0) {
            res.count = RootCount::kZero;
            return res;
        }
        x1 = (-b_doub / (2. * a_doub));
        res.count = RootCount::kOne;
        res.first = x1;
    } else if (d > 0) {
        if (a_doub == 0) {
            res.count = RootCount::kOne;
            res.first = -c_doub / b_doub;
            return res;
        }
        x1 = ((-b_doub + sqrt(d)) / (2 * a_doub));
        x2 = ((-b_doub - sqrt(d)) / (2 * a_doub));
        if (x1 > x2) {
            std::swap(x1, x2);
        }
        res.count = RootCount::kTwo;
        res.first = x1;
        res.second = x2;
    } else if (d < 0) {
        res.count = RootCount::kZero;
    }
    return res;
}
