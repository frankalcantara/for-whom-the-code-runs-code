// cap13/03-closest-pair.cpp
// Solved exercise: closest pair of points in O(n log n).
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o closest 03-closest-pair.cpp

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <print>
#include <vector>

struct P {
    double x, y;
};

double dist2(P a, P b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

double closest_pair_rec(std::vector<P>& pts, int l, int r) {
    if (r - l <= 3) {
        double best = std::numeric_limits<double>::infinity();
        for (int i = l; i < r; ++i)
            for (int j = i + 1; j < r; ++j)
                best = std::min(best, dist2(pts[i], pts[j]));
        std::sort(pts.begin() + l, pts.begin() + r,
                  [](const P& a, const P& b) { return a.y < b.y; });
        return best;
    }

    int mid = (l + r) / 2;
    double mid_x = pts[mid].x;

    double d = std::min(closest_pair_rec(pts, l, mid),
                        closest_pair_rec(pts, mid, r));

    std::inplace_merge(pts.begin() + l, pts.begin() + mid, pts.begin() + r,
                       [](const P& a, const P& b) { return a.y < b.y; });

    std::vector<P> strip;
    for (int i = l; i < r; ++i) {
        double dx = pts[i].x - mid_x;
        if (dx * dx < d) strip.push_back(pts[i]);
    }

    for (int i = 0; i < static_cast<int>(strip.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(strip.size()); ++j) {
            double dy = strip[j].y - strip[i].y;
            if (dy * dy >= d) break;
            d = std::min(d, dist2(strip[i], strip[j]));
        }
    }

    return d;
}

double closest_pair(std::vector<P> pts) {
    if (pts.size() < 2) return 0.0;
    std::sort(pts.begin(), pts.end(), [](const P& a, const P& b) {
        return a.x != b.x ? a.x < b.x : a.y < b.y;
    });
    return std::sqrt(closest_pair_rec(pts, 0, static_cast<int>(pts.size())));
}

int main() {
    std::vector<P> pts1 = {{0, 0}, {3, 4}, {1, 0}, {10, 10}};
    assert(std::abs(closest_pair(pts1) - 1.0) < 1e-9);

    std::vector<P> pts2 = {{0, 0}, {0, 1}, {5, 5}, {10, 0}};
    assert(std::abs(closest_pair(pts2) - 1.0) < 1e-9);

    std::vector<P> pts3 = {{0, 0}, {1, 1}, {2, 2}};
    assert(std::abs(closest_pair(pts3) - std::sqrt(2.0)) < 1e-9);

    std::vector<P> pts4 = {{1, 1}, {1, 1}, {5, 5}};
    assert(closest_pair(pts4) < 1e-9);

    assert(closest_pair({{0, 0}}) == 0.0);

    std::print("03-closest-pair: all assertions passed\n");
    return 0;
}
