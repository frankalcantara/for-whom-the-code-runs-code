// Exercise A6.6 - The Muleteer's Packs
// Session: A6, exercise 6 of 8 (6 hours for the set)
// Theme: chapter 07 revision (meet in the middle), with chapter
//   04's sort + binary search closing the join
// Tags: chapter-07, chapter-04, binary-search, sorting, dynamic-programming, meet-in-the-middle, brute-force
//
// MODEL: subset-sum maximization under a cap - n = 40 closes the
//   door on 2^40 enumeration and the weights (1e15) close it on
//   any DP indexed by value.
// MATH: split the crates into halves A and B of <= 20 each.
//   Enumerate all subset sums of each half: 2^20 ~ 1e6 values per
//   side. Sort B's sums. For each sum x of A with x <= W, binary
//   search the largest sum y of B with y <= W - x; track x + y.
//   Every subset of the full set splits uniquely as (A-part,
//   B-part), so the maximum over joins is the global maximum.
// PROOF: the split is a bijection between subsets of the whole and
//   pairs of half-subsets, and the cap is separable:
//   x + y <= W iff y <= W - x. For fixed x the best partner is the
//   largest y under the bound - exactly what upper_bound minus one
//   returns on the sorted array. No pair is missed, no infeasible
//   pair is taken.
// COMPLEXITY: O(2^(n/2) * n/2) to enumerate (the subset loop
//   touches each bit), O(2^(n/2) log 2^(n/2)) to sort and join:
//   ~2e7 operations. Memory two arrays of 2^20 long long ~ 16 MB.
// TYPES: a single subset sum reaches 40 * 1e15 = 4e16, and
//   x + y <= 8e16: long long, comfortably. W - x cannot underflow
//   (x <= W checked first). Masks are uint32 within a half.
// ALTERNATIVES: value-indexed knapsack DP - the table would need
//   4e16 cells, dead; weight-indexed needs W = 1e18 cells, dead;
//   branch and bound (chapter 14) prunes well on random data but
//   has no worst-case guarantee; greedy largest-first fails the
//   example: it takes 7, neither 5 nor 4 then fits, and it ends
//   at 7 when 5 + 4 = 9 was available.
// EDGE CASES: every crate over W alone (answer 0 - the empty
//   subset is a subset); all crates fit (sum of all); n = 1;
//   duplicate weights; the join where the best A-sum takes no
//   B-partner but a worse one does (the per-x search handles it).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o packs a6-06-muleteers-packs.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

namespace {

// All subset sums of weights[from, to).
std::vector<long long> subset_sums(const std::vector<long long>& weights,
                                   std::size_t from, std::size_t to) {
    const std::size_t bits = to - from;
    std::vector<long long> sums;
    sums.reserve(std::size_t{1} << bits);
    sums.push_back(0);
    for (std::size_t i = 0; i < bits; ++i) {
        const std::size_t have = sums.size();
        for (std::size_t s = 0; s < have; ++s) {
            sums.push_back(sums[s] + weights[from + i]);
        }
    }
    return sums;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    long long capacity = 0;
    std::cin >> n >> capacity;
    std::vector<long long> weights(n);
    for (auto& w : weights) std::cin >> w;

    const std::size_t half = n / 2;
    const std::vector<long long> left = subset_sums(weights, 0, half);
    std::vector<long long> right = subset_sums(weights, half, n);
    std::sort(right.begin(), right.end());

    long long best = 0;
    for (const long long x : left) {
        if (x > capacity) continue;
        const auto it =
            std::upper_bound(right.begin(), right.end(), capacity - x);
        const long long y = *std::prev(it);  // right[0] = 0 always fits
        best = std::max(best, x + y);
    }
    std::print("{}\n", best);
    return 0;
}
