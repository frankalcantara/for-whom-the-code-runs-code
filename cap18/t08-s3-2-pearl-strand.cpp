// Exercise T08.4 - The Pearl Strand
// Session: T08.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 08 (LIS in n log n), using chapter 04 (lower_bound)
// Tags: chapter-08, chapter-04, binary-search, sorting, dynamic-programming, lis
//
// MODEL: longest strictly increasing subsequence. The n^2 DP
//   (dp[i] = 1 + max dp[j] over j < i with s_j < s_i) defines the
//   problem; at n = 2e5 it costs 4e10 steps, so the exercise is
//   really about the patience-sorting compression of that DP.
// MATH: maintain tails[k] = the smallest possible last element of an
//   increasing subsequence of length k+1 over the prefix read so far.
//   tails is strictly increasing (a longer subsequence's best tail
//   cannot be <= a shorter one's, or truncating it would improve the
//   shorter). For each size x, find the first tails entry >= x
//   (lower_bound, strict increase demands >=): replacing it keeps all
//   invariants; appending when none exists extends the best length.
// PROOF: two invariants by induction over the scan: (1) tails[k] is
//   feasible - some increasing subsequence of length k+1 ends exactly
//   at tails[k]; (2) tails[k] is minimal among all such subsequences.
//   The lower_bound replacement preserves both: x extends some
//   subsequence of length k (its predecessor tails[k-1] < x because
//   tails is sorted and tails[k-1] < tails[k] <= old value), and x
//   only lowers the tail. The final size of tails is the LIS length
//   because feasibility gives "at least" and minimality gives "at
//   most".
// COMPLEXITY: O(n log n) time - one binary search per pearl - and
//   O(n) memory for tails. The n^2 DP is the right first draft on
//   paper and the wrong submission at this n.
// TYPES: sizes fit in int (<= 1e9 < 2^31); lengths fit in int. The
//   answer is at most n. Nothing overflows; the risk in this exercise
//   is off-by-one in the bound choice, not width.
// ALTERNATIVES: lower_bound vs upper_bound is the strict/non-strict
//   switch: upper_bound here would count non-decreasing strands,
//   allowing equal pearls - re-read the fashion requirement. A Fenwick
//   tree over compressed values computes LIS too (and counts them),
//   at the same n log n but more code.
// EDGE CASES: strictly decreasing strand (answer 1); all pearls equal
//   (answer 1 - strictness); already graduated (answer n); n = 1;
//   duplicates inside an otherwise increasing strand (must not count
//   twice).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o strand t08-s3-2-pearl-strand.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Festrand.exe t08-s3-2-pearl-strand.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<int> tails;
    tails.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        int x = 0;
        std::cin >> x;
        const auto it = std::ranges::lower_bound(tails, x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    std::print("{}\n", tails.size());
    return 0;
}
