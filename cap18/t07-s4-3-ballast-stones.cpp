// Exercise T07.7 - The Ballast Stones
// Session: T07.S4, exercise 3 of 3 (2 hours for the three; this is the hard one)
// Theme: chapter 07 (meet in the middle), integrating chapter 04
//   (sorting, binary search) and chapter 02 (vectors, algorithms)
// Tags: chapter-07, chapter-04, chapter-02, binary-search, sorting, meet-in-the-middle, bitmask, brute-force
//
// MODEL: subset-sum maximization under a cap. The full space has 2^40
//   ~ 1.1e12 subsets - hopeless directly. Meet in the middle splits the
//   stones into halves A and B (sizes <= 20), enumerates the 2^20
//   subset sums of each, and combines: for each sum s_a from A, the
//   best partner is the largest s_b <= C - s_a, found by binary search
//   in the sorted sums of B.
// MATH: every subset of the n stones is uniquely a (subset of A,
//   subset of B) pair, so max over pairs with s_a + s_b <= C is the
//   true optimum. Enumeration of one half's sums walks masks 0..2^k-1;
//   the lowest-bit trick sums[m] = sums[m & (m-1)] + w[ctz(m)] fills
//   the table in O(2^k) total, one addition per mask.
// PROOF: the split is a bijection between subsets of the whole and
//   pairs of half-subsets (a subset is determined by its two traces).
//   For fixed s_a <= C, the optimal completion is max { s_b : s_b <=
//   C - s_a }, exactly what upper_bound minus one returns on the sorted
//   array; maximizing over all s_a covers every pair. Sums that already
//   exceed C are skipped, which discards no feasible pair.
// COMPLEXITY: O(2^{n/2} * (n/2)) for the combine step's binary
//   searches (about 2e7 comparisons at n = 40), O(2^{n/2}) memory
//   (two vectors of ~1M long long, 8 MB each at the limit). DP over
//   weights is O(nC) with C up to 1e15: not an option.
// TYPES: sums of 40 weights up to 1e9 reach 4e10 and C reaches 1e15:
//   long long everywhere; int truncates both the cap and the sums.
//   Masks fit in unsigned (20 bits per half).
// ALTERNATIVES: branch and bound with a greedy upper bound often wins
//   in practice but has no worst-case guarantee (Chapter 14 will make
//   this respectable); sorting both halves and walking them with two
//   pointers replaces the log factor with a linear merge - the
//   classical refinement, worth knowing, omitted here for clarity.
// EDGE CASES: n = 1 (take it or not); all stones together <= C (take
//   everything); every single stone > C (answer 0); C = 1 with all
//   weights larger (0); duplicate weights; the exact-fit case where
//   the answer equals C.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o ballast t07-s4-3-ballast-stones.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feballast.exe t07-s4-3-ballast-stones.cpp

#include <algorithm>
#include <bit>
#include <iostream>
#include <print>
#include <vector>

namespace {

std::vector<long long> subset_sums(const std::vector<long long>& w) {
    const std::size_t k = w.size();
    std::vector<long long> sums(static_cast<std::size_t>(1) << k, 0);
    for (unsigned mask = 1; mask < sums.size(); ++mask) {
        sums[mask] = sums[mask & (mask - 1)] +
                     w[static_cast<std::size_t>(std::countr_zero(mask))];
    }
    return sums;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long cap = 0;
    std::cin >> n >> cap;

    std::vector<long long> stones(static_cast<std::size_t>(n));
    for (auto& w : stones) {
        std::cin >> w;
    }

    const auto mid = stones.begin() + n / 2;
    auto sums_a = subset_sums({stones.begin(), mid});
    auto sums_b = subset_sums({mid, stones.end()});
    std::ranges::sort(sums_b);

    long long best = 0;
    for (const long long sa : sums_a) {
        if (sa > cap) continue;
        const auto it = std::ranges::upper_bound(sums_b, cap - sa);
        const long long sb = *std::prev(it);  // sums_b[0] == 0, so prev exists
        best = std::max(best, sa + sb);
    }

    std::print("{}\n", best);
    return 0;
}
