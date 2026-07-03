// Exercise T14.5 - The Quiet Council
// Session: T14.S4, exercise 1 of 3 (2 hours for all three)
// Theme: chapter 14 (maximum independent set by meet-in-the-middle),
//   with chapter 05's bit masks and chapter 04's decomposition habit
// Tags: chapter-14, chapter-05, chapter-04, dynamic-programming, bitmask, brute-force
//
// MODEL: maximum independent set in an undirected graph: select as
//   many vertices as possible while selecting no edge's two endpoints.
// MATH: split the vertices into L and R, each of size at most 20.
//   For every subset of R, compute the largest independent subset
//   contained in it. Then enumerate independent subsets A of L.
//   A forbids exactly the vertices of R adjacent to A; the best
//   completion is the precomputed answer inside the remaining R
//   mask. Maximize |A| + best[available].
// PROOF: every valid delegation decomposes uniquely into its left
//   part A and right part B. If A is not independent, the delegation
//   is invalid. If A is independent, every valid B must be contained
//   in the right-side vertices not adjacent to A and must itself be
//   independent; best[available] is exactly the largest such B by
//   the subset DP. Conversely, any A plus any independent B contained
//   in available has no internal conflict and no cross conflict, so
//   it is valid. Exhaustive enumeration of A therefore reaches an
//   optimum.
// COMPLEXITY: O(n * 2^(n/2)) time and O(2^(n/2)) memory. At n = 40,
//   each half has at most 20 vertices, so the million-mask tables are
//   intentional, not accidental.
// TYPES: half masks fit in uint32_t because each half has <= 20
//   vertices. The answer is <= 40, so int is ample. No arithmetic
//   combines input labels beyond indexing.
// ALTERNATIVES: enumerating all 2^40 subsets is too large; a
//   branching vertex-cover solver can be excellent on sparse graphs
//   but has input-sensitive behavior; reducing to maximum clique in
//   the complement changes the story but not the exponential nature.
// EDGE CASES: m = 0 (answer n); complete graph (answer 1); n = 1
//   (answer 1); all conflicts inside one half (the other half is
//   still handled by the available-mask DP); disconnected components
//   require no special case.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o council t14-s4-1-quiet-council.cpp

#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

std::vector<char> independent_table(const std::vector<std::uint32_t>& adj) {
    const int n = static_cast<int>(adj.size());
    const std::uint32_t full = (std::uint32_t{1} << n) - 1;
    std::vector<char> independent(full + 1, 0);
    independent[0] = 1;
    for (std::uint32_t mask = 1; mask <= full; ++mask) {
        const int v = std::countr_zero(mask);
        const std::uint32_t rest = mask & (mask - 1);
        independent[mask] = independent[rest] && ((adj[v] & rest) == 0);
    }
    return independent;
}

std::vector<int> best_right_subsets(const std::vector<std::uint32_t>& adj) {
    const int n = static_cast<int>(adj.size());
    const std::uint32_t full = (std::uint32_t{1} << n) - 1;
    const auto independent = independent_table(adj);

    std::vector<int> best(full + 1, 0);
    for (std::uint32_t mask = 0; mask <= full; ++mask) {
        if (independent[mask]) best[mask] = static_cast<int>(std::popcount(mask));
    }

    for (int bit = 0; bit < n; ++bit) {
        const std::uint32_t flag = std::uint32_t{1} << bit;
        for (std::uint32_t mask = 0; mask <= full; ++mask) {
            if ((mask & flag) != 0) {
                best[mask] = std::max(best[mask], best[mask ^ flag]);
            }
        }
    }
    return best;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    const int left_size = n / 2;
    const int right_size = n - left_size;
    std::vector<std::uint32_t> left_adj(left_size, 0);
    std::vector<std::uint32_t> right_adj(right_size, 0);
    std::vector<std::uint32_t> left_to_right(left_size, 0);

    for (int e = 0; e < m; ++e) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        --u;
        --v;
        if (u > v) std::swap(u, v);

        if (v < left_size) {
            left_adj[u] |= std::uint32_t{1} << v;
            left_adj[v] |= std::uint32_t{1} << u;
        } else if (u >= left_size) {
            u -= left_size;
            v -= left_size;
            right_adj[u] |= std::uint32_t{1} << v;
            right_adj[v] |= std::uint32_t{1} << u;
        } else {
            left_to_right[u] |= std::uint32_t{1} << (v - left_size);
        }
    }

    const auto left_independent = independent_table(left_adj);
    const auto right_best = best_right_subsets(right_adj);
    const std::uint32_t full_left =
        (std::uint32_t{1} << left_size) - 1;
    const std::uint32_t full_right =
        (std::uint32_t{1} << right_size) - 1;

    int answer = 0;
    for (std::uint32_t mask = 0; mask <= full_left; ++mask) {
        if (!left_independent[mask]) continue;

        std::uint32_t forbidden = 0;
        for (std::uint32_t rest = mask; rest != 0; rest &= rest - 1) {
            const int v = std::countr_zero(rest);
            forbidden |= left_to_right[v];
        }
        const std::uint32_t available = full_right & ~forbidden;
        answer = std::max(answer, static_cast<int>(std::popcount(mask)) +
                                      right_best[available]);
    }

    std::print("{}\n", answer);
    return 0;
}
