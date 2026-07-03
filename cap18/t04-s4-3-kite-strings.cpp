// Exercise T04.7 - The Kite Strings
// Session: T04.S4, exercise 3 of 3 (2 hours for the three; this one resists)
// Theme: chapter 04 (binary search on answers over a counted, never
//   materialized, set of n(n-1)/2 values)
// Tags: chapter-04, binary-search-answer, binary-search, sorting
//
// MODEL: the k-th smallest of all pairwise distances |a_i - a_j|.
// MATH: there are up to n(n-1)/2 ~ 5e9 distances; building them is not an
//   algorithm, it is a memory incident. Instead, search the VALUE space.
//   Define count(x) = number of pairs with distance <= x. After sorting
//   the anchors, for each i the partners j > i with a_j - a_i <= x form a
//   contiguous block ending at upper_bound(a, a_i + x); summing the block
//   sizes counts every pair once. count is non-decreasing in x, so
//   "count(x) >= k" is monotone, and the k-th smallest distance is the
//   smallest x with count(x) >= k: the minimization form, first true.
// PROOF: (counting) sortedness makes distance to the right monotone in j,
//   so the qualifying partners of i are exactly a prefix of i's suffix,
//   measured by upper_bound; pairs are counted once with i as the left
//   anchor. (search) let d* be the k-th smallest distance. For x < d*,
//   fewer than k distances are <= x; for x >= d*, at least k are. The
//   first true is therefore exactly d*, and d* is achieved by a real pair
//   because count increases only at achieved distances.
// COMPLEXITY: sort O(n log n); each count O(n log n); ~31 probes over the
//   value range [0, 1e9]: O(n log n log V) ~ 6e7 comparisons. O(n) memory.
// TYPES: positions <= 1e9, so a_i + x <= 2e9 overflows int: anchors and
//   probes live in long long. count reaches ~5e9: long long, the quiet
//   trap of the exercise. k itself reaches ~5e9: read it as long long.
// ALTERNATIVES: materialize-and-nth_element works only while n is small
//   (n <= ~3e4 strains memory at 4 GB); two pointers (Chapter 5) replaces
//   the inner upper_bound and removes one log; the structure of the
//   solution, search the answer and count, survives both upgrades.
// EDGE CASES: k = 1 (minimum adjacent gap after sorting); k = n(n-1)/2
//   (the span max - min); duplicate anchors (distance 0 pairs, the search
//   floor must include 0); all anchors equal.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o kites t04-s4-3-kite-strings.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

long long pairs_within(const std::vector<long long>& a, long long x) {
    long long pairs = 0;
    for (std::size_t i = 0; i + 1 < a.size(); ++i) {
        auto it = std::ranges::upper_bound(a, a[i] + x);
        pairs += (it - a.begin()) - static_cast<long long>(i) - 1;
    }
    return pairs;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long k = 0;
    std::cin >> n >> k;

    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    std::ranges::sort(a);

    long long lo = 0;
    long long hi = a[n - 1] - a[0];
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (pairs_within(a, mid) >= k) hi = mid;  // mid may be the answer
        else                           lo = mid + 1;
    }

    std::print("{}\n", lo);
    return 0;
}
