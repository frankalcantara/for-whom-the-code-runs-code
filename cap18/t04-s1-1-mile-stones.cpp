// Exercise T04.1 - The Mile Stones
// Session: T04.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 04 (lower_bound and its two neighbors; boundary discipline)
// Tags: chapter-04, binary-search, sorting
//
// MODEL: for each query p over a sorted array s, minimize |s_i - p|.
// MATH: lower_bound(p) returns the first stone >= p. The nearest stone is
//   either that one or the one immediately before it: every stone further
//   right is at least as far as the first >= p, and every stone further
//   left is at least as far as the last < p, because the array is sorted.
//   Two candidates, one comparison.
// PROOF: case analysis on the boundary returned by lower_bound. If it is
//   begin(), no stone is < p and the first stone is nearest; if it is
//   end(), no stone is >= p and the last stone is nearest; otherwise both
//   neighbors exist and the smaller absolute difference wins. Sortedness
//   makes all non-candidates dominated, by the monotonicity of distance
//   along a sorted line.
// COMPLEXITY: O(log n) per pilgrim, O((n + q) log n)... the stones are
//   given sorted, so no sort is paid: O(n + q log n) total, O(n) memory.
// TYPES: positions reach 1e18. The difference of two values in [0, 1e18]
//   fits in long long, but only because both are non-negative: the
//   subtraction is always performed larger minus smaller, never through
//   std::abs of a difference that could not exist.
// ALTERNATIVES: scanning all stones per pilgrim is O(nq) = 4e10; sorting
//   the queries and walking both lists with two indices is O(n + q log q)
//   offline, a fine trick the online statement does not require.
// EDGE CASES: pilgrim before the first stone; pilgrim past the last stone;
//   pilgrim exactly on a stone (distance 0); n = 1 (one candidate only);
//   equidistant neighbors (either answer is the same distance).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o stones t04-s1-1-mile-stones.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> stone(n);
    for (auto& s : stone) std::cin >> s;

    while (q-- > 0) {
        long long p = 0;
        std::cin >> p;

        auto it = std::ranges::lower_bound(stone, p);
        long long best = -1;
        if (it != stone.end()) best = *it - p;
        if (it != stone.begin()) {
            long long left = p - *(it - 1);
            if (best == -1 || left < best) best = left;
        }
        std::print("{}\n", best);
    }
    return 0;
}
