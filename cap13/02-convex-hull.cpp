// cap13/02-convex-hull.cpp
// Andrew's monotone chain convex hull and segment intersection.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o convex 02-convex-hull.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feconvex.exe 02-convex-hull.cpp

#include <algorithm>
#include <cassert>
#include <print>
#include <ranges>
#include <vector>

using i64 = long long;

struct P {
    i64 x, y;
    P(i64 x = 0, i64 y = 0) : x(x), y(y) {}
    bool operator<(const P& o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator==(const P& o) const { return x == o.x && y == o.y; }
};

i64 cross(P a, P b, P c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

std::vector<P> convex_hull(std::vector<P> pts) {
    std::ranges::sort(pts, [](const P& a, const P& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    auto duplicates = std::ranges::unique(pts);
    pts.erase(duplicates.begin(), duplicates.end());
    int n = static_cast<int>(pts.size());
    if (n <= 1) return pts;

    std::vector<P> hull;
    hull.reserve(2 * n);

    for (int i = 0; i < n; ++i) {
        while (hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }

    int lower_size = static_cast<int>(hull.size());
    for (int i = n - 2; i >= 0; --i) {
        while (static_cast<int>(hull.size()) > lower_size &&
               cross(hull[hull.size() - 2], hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }

    hull.pop_back();
    return hull;
}

bool on_segment(P p, P a, P b) {
    return cross(a, b, p) == 0 &&
           std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

bool opposite_strict(i64 a, i64 b) {
    return (a > 0 && b < 0) || (a < 0 && b > 0);
}

bool segments_intersect(P a, P b, P c, P d) {
    i64 d1 = cross(c, d, a);
    i64 d2 = cross(c, d, b);
    i64 d3 = cross(a, b, c);
    i64 d4 = cross(a, b, d);

    if (opposite_strict(d1, d2) && opposite_strict(d3, d4)) return true;

    if (d1 == 0 && on_segment(a, c, d)) return true;
    if (d2 == 0 && on_segment(b, c, d)) return true;
    if (d3 == 0 && on_segment(c, a, b)) return true;
    if (d4 == 0 && on_segment(d, a, b)) return true;
    return false;
}

int main() {
    std::vector<P> pts = {{0, 0}, {4, 0}, {4, 4}, {0, 4}, {2, 2}};
    auto hull = convex_hull(pts);
    assert(hull.size() == 4);

    auto h2 = convex_hull({{0, 0}, {3, 0}, {1, 2}});
    assert(h2.size() == 3);

    auto h3 = convex_hull({{0, 0}, {1, 0}, {2, 0}, {3, 0}});
    assert(h3.size() == 2);

    assert(segments_intersect({0, 0}, {2, 2}, {0, 2}, {2, 0}));
    assert(!segments_intersect({0, 0}, {1, 0}, {2, 0}, {3, 0}));
    assert(segments_intersect({0, 0}, {2, 0}, {1, 0}, {3, 0}));
    assert(segments_intersect({0, 0}, {1, 0}, {1, 0}, {1, 1}));
    assert(segments_intersect({0, 0}, {2, 0}, {1, -1}, {1, 1}));
    assert(!segments_intersect({0, 0}, {2, 0}, {0, 1}, {2, 1}));

    std::print("02-convex-hull: all assertions passed\n");
    return 0;
}
