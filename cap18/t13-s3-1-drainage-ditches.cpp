// Exercise T13.3 - The Drainage Ditches
// Session: T13.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 13 (segment intersection by orientation tests,
//   all degeneracies on the table)
// Tags: chapter-13, geometry, parallelism
//
// MODEL: do two closed segments share at least one point?
// MATH: segments AB and CD properly cross iff C and D lie on
//   opposite sides of line AB AND A and B lie on opposite sides of
//   line CD - four cross-product orientation tests. The
//   degenerate cases are exactly the ones with some orientation
//   zero: then the segments intersect iff a collinear endpoint
//   lies within the other segment's bounding box (the on-segment
//   test). No divisions, no slopes, no parallel special case - the
//   slope form is where this exercise goes to die.
// PROOF: (proper case) opposite orientations mean the segment
//   endpoints straddle the other's supporting line; straddling
//   both ways forces the supporting lines' crossing point into
//   both segments (intermediate value on each parameterization).
//   (degenerate case) orientation zero certifies collinearity of
//   three points; for collinear points, membership in the segment
//   equals membership in its axis-aligned bounding box. The four
//   on-segment checks cover every way a shared point can include
//   an endpoint; if no orientation is zero and the straddle test
//   fails, the supporting lines' unique crossing lies outside one
//   segment - no shared point exists.
// COMPLEXITY: O(1) - four crosses, up to four box tests.
// TYPES: coordinate differences reach 2e9; a cross product
//   reaches 8e18: long long with no margin to spare (the chapter's
//   recurring cliff). The comparisons are sign tests - no
//   arithmetic on the results, so no further growth.
// ALTERNATIVES: solving the 2x2 linear system - divisions,
//   epsilons, and a parallel branch that this formulation never
//   needs; floating point - equality of coordinates is the
//   PROBLEM here, and doubles answer it with a shrug.
// EDGE CASES: touching at a single endpoint (YES); collinear with
//   overlap (YES) vs collinear disjoint (NO); one segment
//   degenerate to a point on/off the other; both degenerate
//   (equal or distinct points); T-shaped touch mid-segment (YES);
//   full-range coordinates exercising the 8e18 cross.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o ditches t13-s3-1-drainage-ditches.cpp

#include <algorithm>
#include <iostream>
#include <print>

namespace {

struct Point {
    long long x;
    long long y;
};

long long cross(const Point& o, const Point& a, const Point& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int sign(long long v) { return (v > 0) - (v < 0); }

// p collinear with [a, b] assumed; is p inside the box of [a, b]?
bool on_segment(const Point& a, const Point& b, const Point& p) {
    return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

bool segments_intersect(const Point& a, const Point& b, const Point& c,
                        const Point& d) {
    const int d1 = sign(cross(a, b, c));
    const int d2 = sign(cross(a, b, d));
    const int d3 = sign(cross(c, d, a));
    const int d4 = sign(cross(c, d, b));
    if (d1 * d2 < 0 && d3 * d4 < 0) return true;  // proper crossing
    if (d1 == 0 && on_segment(a, b, c)) return true;
    if (d2 == 0 && on_segment(a, b, d)) return true;
    if (d3 == 0 && on_segment(c, d, a)) return true;
    if (d4 == 0 && on_segment(c, d, b)) return true;
    return false;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Point a{};
    Point b{};
    Point c{};
    Point d{};
    std::cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y;

    std::print("{}\n", segments_intersect(a, b, c, d) ? "YES" : "NO");
    return 0;
}
