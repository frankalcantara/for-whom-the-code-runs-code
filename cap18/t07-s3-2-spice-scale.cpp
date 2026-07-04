// Exercise T07.4 - The Spice Scale
// Session: T07.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 07 (backtracking over a ternary choice tree)
// Tags: chapter-07, dfs, backtracking
//
// MODEL: each brass weight has three fates: left pan (with the parcel),
//   right pan (against it), or the box. The balance is level iff
//   w + sum(left) = sum(right), i.e. w = sum over chosen weights of
//   (+b_i for right, -b_i for left). The question: is w representable
//   as a signed sum of a subset of the weights?
// MATH: the reachable set is { sum c_i b_i : c_i in {-1, 0, +1} }.
//   There are 3^n assignments; with n <= 13 that is 3^13 = 1,594,323,
//   directly enumerable by a depth-first walk of the ternary tree.
// PROOF: the recursion tries all three values of c_i at depth i, so by
//   induction on depth it visits every assignment exactly once; it
//   answers YES iff some leaf evaluates to w. Pruning note: partial sums
//   cannot be bounded usefully without sorting (weights can cancel), but
//   the remaining-mass bound works: if |w - partial| > sum of remaining
//   weights, no completion can close the gap. The prune only discards
//   provably hopeless subtrees, so exhaustiveness is preserved.
// COMPLEXITY: O(3^n) worst case, O(n) stack memory. With the
//   remaining-mass prune, typical instances close far earlier; the
//   worst case at n = 13 still runs in well under a second.
// TYPES: sums of up to 13 weights of 1e9 reach 1.3e10 and partial
//   signed sums range in [-1.3e10, +1.3e10]: long long is mandatory,
//   int overflows. The suffix-sum array is long long for the same
//   reason.
// ALTERNATIVES: meet in the middle splits 3^13 into 3^7 * 3^6 with
//   sorted lookup - the heavy machine for n around 30, overkill here.
//   DP over reachable sums needs offsetting and a 2.6e10-wide value
//   range: hopeless as an array, and a hash set of reachable sums is
//   just the enumeration again with extra memory.
// EDGE CASES: w equal to one weight; w equal to the total mass (all
//   weights on the right); representable only with subtraction (the
//   example); unreachable w (parity or gap); n = 1; duplicate weights.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o scale t07-s3-2-spice-scale.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fescale.exe t07-s3-2-spice-scale.cpp

#include <cstdlib>
#include <iostream>
#include <print>
#include <vector>

namespace {

bool reachable(int i, long long remaining_target,
               const std::vector<long long>& weights,
               const std::vector<long long>& suffix_mass) {
    if (remaining_target == 0) return true;
    if (i == static_cast<int>(weights.size())) return false;
    if (std::llabs(remaining_target) > suffix_mass[static_cast<std::size_t>(i)]) {
        return false;  // not enough mass left to close the gap
    }
    const long long b = weights[static_cast<std::size_t>(i)];
    return reachable(i + 1, remaining_target - b, weights, suffix_mass) ||
           reachable(i + 1, remaining_target + b, weights, suffix_mass) ||
           reachable(i + 1, remaining_target, weights, suffix_mass);
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long w = 0;
    std::cin >> n >> w;

    std::vector<long long> weights(static_cast<std::size_t>(n));
    for (auto& b : weights) {
        std::cin >> b;
    }

    std::vector<long long> suffix_mass(static_cast<std::size_t>(n) + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        suffix_mass[static_cast<std::size_t>(i)] =
            suffix_mass[static_cast<std::size_t>(i) + 1] + weights[static_cast<std::size_t>(i)];
    }

    std::print("{}\n", reachable(0, w, weights, suffix_mass) ? "YES" : "NO");
    return 0;
}
