// cap13/01-vectors-cross.cpp
// Points, 2D vectors, cross product, polygon area, and orientation.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o vectors 01-vectors-cross.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fevectors.exe 01-vectors-cross.cpp

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <print>
#include <vector>

using i64 = long long;

struct P {
    i64 x, y;
    P(i64 x = 0, i64 y = 0) : x(x), y(y) {}
    P operator+(const P& o) const { return {x + o.x, y + o.y}; }
    P operator-(const P& o) const { return {x - o.x, y - o.y}; }
    P operator*(i64 k) const { return {x * k, y * k}; }
    bool operator==(const P& o) const { return x == o.x && y == o.y; }
    bool operator<(const P& o) const { return x != o.x ? x < o.x : y < o.y; }
};

i64 cross2(P u, P v) {
    return u.x * v.y - u.y * v.x;
}

i64 cross(P a, P b, P c) {
    return cross2(b - a, c - a);
}

i64 dot(P u, P v) {
    return u.x * v.x + u.y * v.y;
}

i64 signed_area2(const std::vector<P>& poly) {
    int n = static_cast<int>(poly.size());
    i64 s = 0;
    for (int i = 0; i < n; ++i) {
        const P& a = poly[i];
        const P& b = poly[(i + 1) % n];
        s += a.x * b.y - b.x * a.y;
    }
    return s;
}

bool on_segment(P p, P a, P b) {
    return cross(a, b, p) == 0 &&
           std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

int main() {
    P a{0, 0}, b{1, 0}, c{0, 1};
    assert(cross(a, b, c) > 0);
    assert(cross(a, b, {2, 0}) == 0);
    assert(cross(a, b, {0, -1}) < 0);

    assert(dot({3, 4}, {5, 6}) == 39);
    assert(cross2({1, 0}, {0, 1}) == 1);

    std::vector<P> tri = {{0, 0}, {4, 0}, {0, 3}};
    assert(signed_area2(tri) == 12);

    std::vector<P> sq = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    assert(signed_area2(sq) == 2);

    std::vector<P> sq_cw = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
    assert(signed_area2(sq_cw) == -2);

    std::vector<P> quad = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    assert(std::abs(signed_area2(quad)) == 32);

    assert(on_segment({1, 0}, {0, 0}, {2, 0}));
    assert(!on_segment({3, 0}, {0, 0}, {2, 0}));

    std::print("01-vectors-cross: all assertions passed\n");
    return 0;
}
