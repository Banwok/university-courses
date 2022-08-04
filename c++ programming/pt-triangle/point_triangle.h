#pragma once

#include <stdexcept>
#include <algorithm>

struct Point {
    int x, y;
};

struct Triangle {
    Point a, b, c;
};

int64_t Sid(const Point& p1, const Point& p2, const Point& p) {
    int64_t p1x = static_cast<int64_t>(p1.x);
    int64_t p2x = static_cast<int64_t>(p2.x);
    int64_t px = static_cast<int64_t>(p.x);
    int64_t p1y = static_cast<int64_t>(p1.y);
    int64_t p2y = static_cast<int64_t>(p2.y);
    int64_t py = static_cast<int64_t>(p.y);

    int64_t res = (((px - p1x) * (p2y - p1y)) - ((py - p1y) * (p2x - p1x)));
    return res;
}

bool LineChek(const Point& p1, const Point& p2, const Point& p) {
    int64_t p1x = static_cast<int64_t>(p1.x);
    int64_t p2x = static_cast<int64_t>(p2.x);
    int64_t px = static_cast<int64_t>(p.x);
    int64_t p1y = static_cast<int64_t>(p1.y);
    int64_t p2y = static_cast<int64_t>(p2.y);
    int64_t py = static_cast<int64_t>(p.y);
    if (std::min(p1y, p2y) <= py && (std::max(p1y, p2y) >= py) && std::min(p1x, p2x) <= px &&
        std::max(p1x, p2x) >= px) {
        return true;
    }
    return false;
}

bool ZeroCheck(const Point& p1, const Point& p2, const Point& p3) {
    if (p1.x == p2.x && p2.x == p3.x && p1.y == p2.y && p2.y == p3.y) {
        return true;
    }
    return false;
}

inline bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    if (ZeroCheck(t.a, t.b, t.c)) {
        return false;
    }
    int64_t line_chek = Sid(t.a, t.b, t.c);
    if (line_chek == 0) {
        bool lc1 = LineChek(t.a, t.b, pt);
        bool lc2 = LineChek(t.b, t.c, pt);
        bool lc3 = LineChek(t.c, t.a, pt);
        return lc1 || lc2 || lc3;
    }
    int64_t chek1 = Sid(t.a, t.b, pt);
    int64_t chek2 = Sid(t.b, t.c, pt);
    int64_t chek3 = Sid(t.c, t.a, pt);
    if (chek1 >= 0 && chek2 >= 0 && chek3 >= 0) {
        return true;
    } else if (chek1 <= 0 && chek2 <= 0 && chek3 <= 0) {
        return true;
    }
    return false;
}