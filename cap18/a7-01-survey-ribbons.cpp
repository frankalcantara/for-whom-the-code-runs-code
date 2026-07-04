// Exercise A7.1 - The Survey Ribbons
// Session: A7, exercise 1 of 9
// Theme: chapter 13 (segment intersection)
// Tags: chapter-13, geometry
//
// MODEL: each query asks whether two closed line segments in the
//   integer plane have a nonempty intersection.
// MATH: use oriented area. Points a, b, c have orientation
//   sign(cross(b - a, c - a)). Two segments properly cross when
//   the endpoints of each segment lie on opposite sides of the
//   other segment. Collinear cases are handled by checking whether
//   an endpoint lies inside the other segment's bounding box.
// PROOF: orientation classifies which side of a directed line a
//   point occupies. If both pairs of orientations straddle, the
//   infinite lines meet inside both closed segments. If any
//   orientation is zero, the only remaining way to intersect is
//   collinear containment of an endpoint, which the bounding-box
//   test checks exactly.
// COMPLEXITY: O(q) time and O(1) extra memory.
// TYPES: coordinate differences are at most 2e9. Products are at
//   most 4e18 and a subtraction stays within signed 64-bit limits
//   under the statement bounds. The count is at most q, so int
//   would fit; long long is used for uniform output arithmetic.
// ALTERNATIVES: floating-point geometry would add rounding risk for
//   no benefit; sweep line is for many segments in one shared set,
//   not independent pairs.
// EDGE CASES: endpoint touch; overlapping collinear segments;
//   disjoint collinear segments; vertical and horizontal segments;
//   reversed endpoints.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o ribbons a7-01-survey-ribbons.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feribbons.exe a7-01-survey-ribbons.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>

namespace {

struct Point {
    long long x;
    long long y;
};

long long cross(Point a, Point b, Point c) {
    const long long ux = b.x - a.x;
    const long long uy = b.y - a.y;
    const long long vx = c.x - a.x;
    const long long vy = c.y - a.y;
    return ux * vy - uy * vx;
}

int sign(long long value) {
    if (value < 0) return -1;
    if (value > 0) return 1;
    return 0;
}

bool on_segment(Point a, Point b, Point p) {
    return cross(a, b, p) == 0 &&
           std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

bool intersects(Point a, Point b, Point c, Point d) {
    const int o1 = sign(cross(a, b, c));
    const int o2 = sign(cross(a, b, d));
    const int o3 = sign(cross(c, d, a));
    const int o4 = sign(cross(c, d, b));

    if (o1 == 0 && on_segment(a, b, c)) return true;
    if (o2 == 0 && on_segment(a, b, d)) return true;
    if (o3 == 0 && on_segment(c, d, a)) return true;
    if (o4 == 0 && on_segment(c, d, b)) return true;
    return o1 != o2 && o3 != o4;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q = 0;
    std::cin >> q;
    long long answer = 0;
    for (int i = 0; i < q; ++i) {
        Point a, b, c, d;
        std::cin >> a.x >> a.y >> b.x >> b.y;
        std::cin >> c.x >> c.y >> d.x >> d.y;
        if (intersects(a, b, c, d)) ++answer;
    }

    std::print("{}\n", answer);
    return 0;
}
