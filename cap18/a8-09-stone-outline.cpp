// Exercise A8.9 - The Stone Outline
// Session: A8, exercise 9 of 10
// Theme: chapter 13 (shoelace formula)
// Tags: chapter-13, geometry
//
// MODEL: compute twice the area of a simple polygon from ordered
//   vertices.
// MATH: twice the signed area is sum_i x_i*y_{i+1}-y_i*x_{i+1}.
// PROOF: shoelace decomposes the polygon into oriented trapezoids
//   relative to the origin; interior contributions cancel and the
//   boundary sum remains. Absolute value removes orientation.
// COMPLEXITY: O(n) time and O(1) extra memory.
// TYPES: under the stated coordinate bound the accumulated sum fits
//   int64_t.
// ALTERNATIVES: triangulation is equivalent but more code.
// EDGE CASES: clockwise order; counterclockwise order; axis-aligned
//   rectangles; negative coordinates.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o outline a8-09-stone-outline.cpp

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <print>
#include <vector>

struct Point {
    std::int64_t x = 0;
    std::int64_t y = 0;
};

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<Point> p(n);
    for (Point& point : p) std::cin >> point.x >> point.y;
    std::int64_t twice = 0;
    for (int i = 0; i < n; ++i) {
        const Point& a = p[i];
        const Point& b = p[(i + 1) % n];
        twice += a.x * b.y - a.y * b.x;
    }
    if (twice < 0) twice = -twice;
    std::print("{}\n", twice);
    return 0;
}
