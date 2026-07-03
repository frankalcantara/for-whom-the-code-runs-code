// Exercise T13.6 - The Well Diggers
// Session: T13.S4, exercise 2 of 3 (2 hours for all three)
// Theme: chapter 13 (closest pair by divide and conquer)
// Tags: chapter-13, sorting, geometry
//
// MODEL: minimum squared distance over all point pairs.
// MATH: sort by x once. Recurse on halves split at the median x;
//   let d^2 be the best of the two halves. A cross pair can beat
//   d^2 only if both points lie within d of the split line - the
//   STRIP. Walk the strip in y order (maintained by merging the
//   halves' y-sorted lists as the recursion returns): each strip
//   point needs comparing only against the next few strip
//   neighbors, because a d x 2d rectangle of the strip holds at
//   most 8 points at pairwise distance >= d (pack them into eight
//   d/2-half-open boxes; two in one box would be closer than d).
// PROOF: the packing argument bounds the inner loop at O(1) per
//   strip point (break when the y-gap alone reaches d^2, since
//   dy^2 >= d^2 makes the full distance no better). Every pair is
//   considered: same-half pairs by induction, cross pairs in the
//   strip if they can possibly win. The merge keeps y-order as an
//   invariant, making the strip walk linear.
// COMPLEXITY: T(n) = 2T(n/2) + O(n) = O(n log n); O(n) extra for
//   the merge buffers.
// TYPES: coordinate differences reach 2e9, squares 4e18, their
//   SUM 8e18: at the very lip of long long, the chapter's cliff
//   once more. The y-gap early exit squares a difference already
//   known to be < some candidate <= 8e18 - but the FIRST candidate
//   is initialized to the max sentinel, so the strip break
//   compares dy*dy against the sentinel: dy can be 4e9? No - dy is
//   a difference of coordinates, <= 2e9, square 4e18: safe.
// ALTERNATIVES: the sweep with a std::set ordered by y (insert
//   points left to right, search a d-box back) - same bound,
//   beautiful, chapter 6 flavored; randomized grid hashing -
//   expected O(n), more machinery; O(n^2) - 4e10, dead at 2e5.
// EDGE CASES: duplicate points (0 - the answer's floor);
//   collinear points; two points only; the all-corners spread
//   exercising 8e18; clustered points where the strip holds
//   everything (the packing bound is what saves the run time).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o wells t13-s4-2-well-diggers.cpp

#include <algorithm>
#include <iostream>
#include <limits>
#include <print>
#include <utility>
#include <vector>

namespace {

using Point = std::pair<long long, long long>;  // (x, y)

long long sq(long long v) { return v * v; }

long long dist2(const Point& a, const Point& b) {
    return sq(a.first - b.first) + sq(a.second - b.second);
}

// pts[lo, hi) sorted by x on entry, by y on exit. Returns best d^2.
long long solve(std::vector<Point>& pts, std::vector<Point>& tmp,
                std::size_t lo, std::size_t hi) {
    const std::size_t count = hi - lo;
    if (count <= 3) {  // base case: brute force, then y-sort
        long long best = std::numeric_limits<long long>::max();
        for (std::size_t i = lo; i < hi; ++i) {
            for (std::size_t j = i + 1; j < hi; ++j) {
                best = std::min(best, dist2(pts[i], pts[j]));
            }
        }
        std::sort(pts.begin() + static_cast<long long>(lo),
                  pts.begin() + static_cast<long long>(hi),
                  [](const Point& a, const Point& b) {
                      return a.second < b.second;
                  });
        return best;
    }

    const std::size_t mid = lo + count / 2;
    const long long split_x = pts[mid].first;
    long long best = std::min(solve(pts, tmp, lo, mid),
                              solve(pts, tmp, mid, hi));

    // Merge halves by y into tmp, then copy back.
    std::merge(pts.begin() + static_cast<long long>(lo),
               pts.begin() + static_cast<long long>(mid),
               pts.begin() + static_cast<long long>(mid),
               pts.begin() + static_cast<long long>(hi),
               tmp.begin() + static_cast<long long>(lo),
               [](const Point& a, const Point& b) {
                   return a.second < b.second;
               });
    std::copy(tmp.begin() + static_cast<long long>(lo),
              tmp.begin() + static_cast<long long>(hi),
              pts.begin() + static_cast<long long>(lo));

    // Strip walk in y order.
    std::vector<Point> strip;
    strip.reserve(count);
    for (std::size_t i = lo; i < hi; ++i) {
        const long long dx = pts[i].first - split_x;
        if (sq(dx) < best) strip.push_back(pts[i]);
    }
    for (std::size_t i = 0; i < strip.size(); ++i) {
        for (std::size_t j = i + 1; j < strip.size(); ++j) {
            const long long dy = strip[j].second - strip[i].second;
            if (sq(dy) >= best) break;  // nothing below can win
            best = std::min(best, dist2(strip[i], strip[j]));
        }
    }
    return best;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;
    std::vector<Point> pts(n);
    for (auto& [x, y] : pts) std::cin >> x >> y;

    std::sort(pts.begin(), pts.end());
    std::vector<Point> tmp(n);
    std::print("{}\n", solve(pts, tmp, 0, n));
    return 0;
}
