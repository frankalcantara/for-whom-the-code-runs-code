// Exercise A6.3 - The Flooded Crossings
// Session: A6, exercise 3 of 8 (6 hours for the set; this is the
//   integration problem)
// Theme: chapters 11 + 10 + 04, integration (lattice path counting
//   with binomials mod p; first-forbidden-cell DP over the obstacles
//   sorted lexicographically)
// Tags: chapter-11, chapter-10, chapter-04, sorting, dynamic-programming, modular-arithmetic
//
// MODEL: monotone lattice paths avoiding k forbidden cells in a
//   grid too large to sweep - count by combinatorics, correct by
//   DP over the obstacles only.
// MATH: paths between (r1,c1) and (r2,c2) going south/east number
//   C((r2-r1) + (c2-c1), r2-r1) - choose which steps go south.
//   Sort the flooded cells by (row, column). Let bad[i] = number
//   of paths from start to cell i whose FIRST flooded cell is i:
//   bad[i] = P(start -> i) - sum_{j before i} bad[j] * P(j -> i),
//   where "before" means reachable: row_j <= row_i and
//   col_j <= col_i. Answer:
//   P(start -> end) - sum_i bad[i] * P(i -> end).
// PROOF: partition the wet paths by the first flooded cell they
//   touch - the classes are disjoint and exhaustive, so
//   subtracting their sizes from the total counts exactly the dry
//   paths. bad[i] is correct by strong induction in sorted order:
//   start-to-i paths either avoid all earlier flooded cells (the
//   bad[i] we want) or have a first flooded cell j != i, j
//   reachable-before i - subtracted exactly once each.
// COMPLEXITY: O(n) factorial tables (n = r + c), O(k^2) for the
//   DP (4e6 at k = 2000), O(k log k) sort. Memory O(n + k).
// TYPES: r + c <= 2e6 table indices; residue products ~1e18 in
//   long long; coordinates fit int but live in long long to keep
//   the differences and binomial arguments uniform.
// ALTERNATIVES: cell-by-cell DP over the grid is O(rc) = 1e12
//   cells - dead, which is the point of the exercise; with SMALL
//   grids it would be the simpler answer. Inclusion-exclusion over
//   subsets of obstacles is O(2^k); the first-cell DP is IE
//   reorganized so it telescopes.
// EDGE CASES: k = 0 (single binomial); a flooded cell off every
//   path (contributes nothing - bad[i] becomes 0 only if
//   unreachable from start, and P(i -> end) = 0 kills the rest);
//   flooded cells in the same row or column as each other; r = 1
//   (single corridor - any flooded cell zeroes the answer);
//   duplicate flooded cells excluded by the contract; answers kept
//   non-negative after each subtraction.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o crossings a6-03-flooded-crossings.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace {

constexpr long long kMod = 1'000'000'007;

long long power_mod(long long base, long long exponent) {
    long long result = 1;
    base %= kMod;
    while (exponent > 0) {
        if ((exponent & 1) == 1) result = result * base % kMod;
        base = base * base % kMod;
        exponent >>= 1;
    }
    return result;
}

std::vector<long long> fact;
std::vector<long long> inv_fact;

void build_tables(std::size_t size) {
    fact.assign(size, 1);
    for (std::size_t i = 1; i < size; ++i) {
        fact[i] = fact[i - 1] * static_cast<long long>(i) % kMod;
    }
    inv_fact.assign(size, 1);
    inv_fact[size - 1] = power_mod(fact[size - 1], kMod - 2);
    for (std::size_t i = size - 1; i > 0; --i) {
        inv_fact[i - 1] = inv_fact[i] * static_cast<long long>(i) % kMod;
    }
}

// Monotone paths from (r1, c1) to (r2, c2); 0 if not reachable.
long long paths(long long r1, long long c1, long long r2, long long c2) {
    if (r2 < r1 || c2 < c1) return 0;
    const auto down = static_cast<std::size_t>(r2 - r1);
    const auto right = static_cast<std::size_t>(c2 - c1);
    return fact[down + right] * inv_fact[down] % kMod * inv_fact[right] % kMod;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long r = 0;
    long long c = 0;
    std::size_t k = 0;
    std::cin >> r >> c >> k;

    std::vector<std::pair<long long, long long>> wet(k);
    for (auto& [row, col] : wet) std::cin >> row >> col;
    std::sort(wet.begin(), wet.end());

    build_tables(static_cast<std::size_t>(r + c));  // need (r-1)+(c-1)+1

    std::vector<long long> bad(k, 0);
    for (std::size_t i = 0; i < k; ++i) {
        bad[i] = paths(1, 1, wet[i].first, wet[i].second);
        for (std::size_t j = 0; j < i; ++j) {
            const long long via = bad[j] * paths(wet[j].first, wet[j].second,
                                                 wet[i].first, wet[i].second) %
                                  kMod;
            bad[i] = (bad[i] - via + kMod) % kMod;
        }
    }

    long long answer = paths(1, 1, r, c);
    for (std::size_t i = 0; i < k; ++i) {
        const long long via =
            bad[i] * paths(wet[i].first, wet[i].second, r, c) % kMod;
        answer = (answer - via + kMod) % kMod;
    }
    std::print("{}\n", answer);
    return 0;
}
