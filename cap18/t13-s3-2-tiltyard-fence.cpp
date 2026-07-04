// Exercise T13.4 - The Tiltyard Fence
// Session: T13.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 13 (polygon convexity via consecutive cross
//   products; the winding subtlety)
// Tags: chapter-13
//
// MODEL: is a closed polygonal chain a convex polygon? Collinear
//   consecutive edges allowed, self-intersection not.
// MATH: three checks over the cyclic edge vectors e_i:
//   (1) consistent turning - sign(cross(e_i, e_{i+1})) is never
//       both +1 and -1 around the cycle;
//   (2) no reversals - cross = 0 with dot(e_i, e_{i+1}) < 0 means
//       the rail turns 180 degrees on the spot: a spike, rejected;
//   (3) single winding - the edge direction, classified as "upper"
//       (dy > 0, or dy = 0 and dx > 0) or "lower", alternates
//       upper -> lower exactly once around the cycle. Checks 1+2
//       allow a chain that spirals twice before closing; such a
//       chain's direction vector completes TWO revolutions and
//       crosses the half-plane boundary twice - check 3 is what
//       rejects it.
// PROOF: a simple closed polygon with all turns weakly in one
//   direction and total turning +-360 degrees is convex (the
//   discrete four-vertex/rotation argument: supporting lines
//   advance monotonically). Conversely a convex polygon satisfies
//   all three by inspection. Checks 1-3 are exactly "weak one-way
//   turning, no degenerate reversal, total turning one
//   revolution": together equivalent to convexity, and a
//   double-wound or self-crossing fence fails 3 (or 1).
// COMPLEXITY: O(n) time, O(1) memory beyond the vertex list.
// TYPES: crosses and dots of differences reach 8e18: long long at
//   the cliff edge again. Only signs are consumed.
// ALTERNATIVES: compute the convex hull and compare vertex counts
//   - O(n log n), and equality testing against collinear-kept
//   vertices is fiddlier than it sounds; summing turning angles in
//   floating point - 1e5 additions of atan2 noise deciding a
//   YES/NO is exactly the wrong tool.
// EDGE CASES: collinear consecutive posts (allowed - cross 0, dot
//   > 0); a spike (rejected by check 2); all posts collinear
//   ("polygon" of zero area - rejected: any closing chain
//   reverses); clockwise input (symmetric); double-wound input
//   (rejected by check 3); n = 3.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o tiltyard t13-s3-2-tiltyard-fence.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fetiltyard.exe t13-s3-2-tiltyard-fence.cpp

#include <iostream>
#include <print>
#include <vector>

namespace {

struct Vec {
    long long x;
    long long y;
};

int sign(long long v) { return (v > 0) - (v < 0); }

bool upper_half(const Vec& e) {
    return e.y > 0 || (e.y == 0 && e.x > 0);
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::cin >> n;
    std::vector<long long> xs(n);
    std::vector<long long> ys(n);
    for (std::size_t i = 0; i < n; ++i) std::cin >> xs[i] >> ys[i];

    std::vector<Vec> edge(n);
    for (std::size_t i = 0; i < n; ++i) {
        const std::size_t j = (i + 1) % n;
        edge[i] = {xs[j] - xs[i], ys[j] - ys[i]};
    }

    bool ok = true;
    int turn_sign = 0;       // first nonzero cross sign seen
    int half_switches = 0;   // upper -> lower transitions
    for (std::size_t i = 0; i < n && ok; ++i) {
        const Vec& e = edge[i];
        const Vec& f = edge[(i + 1) % n];
        const long long cr = e.x * f.y - e.y * f.x;
        const long long dot = e.x * f.x + e.y * f.y;
        if (cr == 0) {
            if (dot < 0) ok = false;  // 180-degree spike
        } else {
            const int s = sign(cr);
            if (turn_sign == 0) turn_sign = s;
            if (s != turn_sign) ok = false;  // turns both ways
        }
        if (upper_half(e) && !upper_half(f)) ++half_switches;
    }
    if (turn_sign == 0) ok = false;       // never turns: no area
    if (half_switches != 1) ok = false;   // must wind exactly once

    std::print("{}\n", ok ? "YES" : "NO");
    return 0;
}
