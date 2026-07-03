// Exercise T13.2 - The Sheepfold Stakes
// Session: T13.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 13 (convex hull by monotone chain; shoelace on
//   the hull), with chapter 04's sort
// Tags: chapter-13, chapter-04, sorting, geometry
//
// MODEL: convex hull of n points, then its doubled area.
// MATH: monotone chain - sort points lexicographically; build the
//   lower hull left to right, popping the last point while the
//   last three turn non-left (cross <= 0); build the upper hull
//   right to left the same way; concatenate. The doubled area is
//   the shoelace sum over the hull, fanned from hull[0] so every
//   triangle (hull[0], hull[i], hull[i+1]) has non-negative
//   doubled area and the partial sums never wander.
// PROOF: (hull) after processing point p, the chain is the lower
//   hull of the points so far: popping removes exactly the
//   vertices made non-extreme by p (a non-left turn certifies the
//   middle point lies on or above segment ends), and induction
//   carries it to the end; symmetric for the upper chain.
//   (area) the fan triangulation of a convex polygon partitions
//   it, and each cross product is twice one triangle.
// COMPLEXITY: O(n log n) for the sort, O(n) for the chains (each
//   point pushed and popped at most twice). O(n) memory.
// TYPES: the working trap. Coordinate differences reach 2e9, a
//   cross product reaches 2e9 * 2e9 * 2 = 8e18: INSIDE long long
//   (9.2e18) but with no margin - one more bit of coordinate and
//   this file needs __int128. The doubled hull area is likewise
//   <= 8e18 (the bounding box ceiling), and the fan keeps partial
//   sums below that ceiling instead of letting term-by-term
//   shoelace partials drift past it. The TYPES section is the
//   exercise.
// ALTERNATIVES: Graham scan with angular sort - same bound, a
//   sort comparator with more failure modes; gift wrapping -
//   O(nh), loses when the hull is large; computing area in double
//   - 8e18 does not fit in 53 bits: wrong answers on exactly the
//   adversarial inputs that matter.
// EDGE CASES: all points collinear (hull degenerates to a
//   segment: 0); n <= 2 (0); duplicate points (sorted adjacency
//   collapses them via cross = 0 pops); the full-range square
//   (the 8e18 ceiling exercised); a hull of exactly 3 points.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o sheepfold t13-s2-1-sheepfold-stakes.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace {

using Point = std::pair<long long, long long>;

long long cross(const Point& o, const Point& a, const Point& b) {
    return (a.first - o.first) * (b.second - o.second) -
           (a.second - o.second) * (b.first - o.first);
}

std::vector<Point> convex_hull(std::vector<Point> pts) {
    std::sort(pts.begin(), pts.end());
    pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
    const std::size_t n = pts.size();
    if (n < 3) return pts;
    std::vector<Point> hull(2 * n);
    std::size_t k = 0;
    for (std::size_t i = 0; i < n; ++i) {  // lower chain
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) --k;
        hull[k++] = pts[i];
    }
    const std::size_t lower = k + 1;
    for (std::size_t i = n - 1; i-- > 0;) {  // upper chain
        while (k >= lower && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) --k;
        hull[k++] = pts[i];
    }
    hull.resize(k - 1);  // last point repeats the first
    return hull;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;
    std::vector<Point> pts(n);
    for (auto& [x, y] : pts) std::cin >> x >> y;

    const std::vector<Point> hull = convex_hull(std::move(pts));
    if (hull.size() < 3) {
        std::print("0\n");
        return 0;
    }
    long long doubled = 0;
    for (std::size_t i = 1; i + 1 < hull.size(); ++i) {
        doubled += cross(hull[0], hull[i], hull[i + 1]);  // fan: each >= 0
    }
    std::print("{}\n", doubled);
    return 0;
}
