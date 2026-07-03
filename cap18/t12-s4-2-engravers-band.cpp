// Exercise T12.6 - The Engraver's Band
// Session: T12.S4, exercise 2 of 3 (2 hours for all three)
// Theme: chapter 12 (suffix array, Kasai's LCP, counting distinct
//   substrings)
// Tags: chapter-12, sorting, string-algorithms, suffix-array
//
// MODEL: number of distinct non-empty substrings of s.
// MATH: every substring is a prefix of a suffix. Walk the suffixes
//   in suffix-array order: suffix sa[i] of length n - sa[i]
//   contributes that many prefixes, of which lcp[i] (its longest
//   common prefix with the previous suffix in order) were already
//   contributed. Total:
//   sum_i (n - sa[i]) - sum_i lcp[i] = n(n+1)/2 - sum lcp.
// PROOF: a substring's occurrences as prefixes of suffixes form a
//   CONTIGUOUS run in suffix-array order (all suffixes starting
//   with it sort together); charging it to the first suffix of its
//   run counts it exactly once, and "first of the run" is exactly
//   "not shared with the previous suffix", i.e. longer than
//   lcp[i]. The two sums implement the charge.
// COMPLEXITY: suffix array by rank-pair doubling with std::sort:
//   O(n log^2 n) ~ 1e5 * 17^2 ~ 3e7 comparisons-ish - comfortable;
//   Kasai O(n). Memory O(n). (Radix doubling gives O(n log n),
//   SA-IS O(n); neither is needed at 1e5.)
// TYPES: n(n+1)/2 at n = 1e5 is ~5e9: OVERFLOWS int32 - the
//   exercise's one landmine; long long for the answer and the
//   subtraction. Ranks and indices fit int32.
// ALTERNATIVES: suffix automaton counts distinct substrings as
//   sum over states of len(v) - len(link(v)) in O(n) - elegant,
//   not in the book's toolkit; hashing each substring - O(n^2)
//   states, dead at 1e5; eertree counts only palindromic ones.
// EDGE CASES: |s| = 1 (1); all letters equal (answer n - aaa
//   example: a, aa, aaa); the empty substring excluded by the
//   "non-empty" in the contract; n = 1e5 overflow drill.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o band t12-s4-2-engravers-band.cpp

#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <string>
#include <vector>

namespace {

std::vector<int> suffix_array(const std::string& s) {
    const int n = static_cast<int>(s.size());
    std::vector<int> sa(static_cast<std::size_t>(n));
    std::vector<int> rank(static_cast<std::size_t>(n));
    std::vector<int> tmp(static_cast<std::size_t>(n));
    std::iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; ++i) {
        rank[static_cast<std::size_t>(i)] = s[static_cast<std::size_t>(i)];
    }
    for (int len = 1;; len *= 2) {
        const auto compare = [&](int a, int b) {
            const auto ra = rank[static_cast<std::size_t>(a)];
            const auto rb = rank[static_cast<std::size_t>(b)];
            if (ra != rb) return ra < rb;
            const int sa2 = a + len < n ? rank[static_cast<std::size_t>(a + len)] : -1;
            const int sb2 = b + len < n ? rank[static_cast<std::size_t>(b + len)] : -1;
            return sa2 < sb2;
        };
        std::sort(sa.begin(), sa.end(), compare);
        tmp[static_cast<std::size_t>(sa[0])] = 0;
        for (int i = 1; i < n; ++i) {
            const auto prev = static_cast<std::size_t>(sa[static_cast<std::size_t>(i - 1)]);
            const auto cur = static_cast<std::size_t>(sa[static_cast<std::size_t>(i)]);
            tmp[cur] = tmp[prev] + (compare(static_cast<int>(prev), static_cast<int>(cur)) ? 1 : 0);
        }
        rank = tmp;
        if (rank[static_cast<std::size_t>(sa[static_cast<std::size_t>(n - 1)])] == n - 1) break;
    }
    return sa;
}

std::vector<int> kasai_lcp(const std::string& s, const std::vector<int>& sa) {
    const int n = static_cast<int>(s.size());
    std::vector<int> pos(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) pos[static_cast<std::size_t>(sa[static_cast<std::size_t>(i)])] = i;
    std::vector<int> lcp(static_cast<std::size_t>(n), 0);  // lcp[i]: sa[i-1] vs sa[i]
    int h = 0;
    for (int i = 0; i < n; ++i) {
        const int r = pos[static_cast<std::size_t>(i)];
        if (r == 0) { h = 0; continue; }
        const int j = sa[static_cast<std::size_t>(r - 1)];
        if (h > 0) --h;
        while (i + h < n && j + h < n &&
               s[static_cast<std::size_t>(i + h)] == s[static_cast<std::size_t>(j + h)]) {
            ++h;
        }
        lcp[static_cast<std::size_t>(r)] = h;
    }
    return lcp;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    const long long n = static_cast<long long>(s.size());
    const std::vector<int> sa = suffix_array(s);
    const std::vector<int> lcp = kasai_lcp(s, sa);

    long long total = n * (n + 1) / 2;  // ~5e9 at n = 1e5: long long
    for (const int v : lcp) total -= v;
    std::print("{}\n", total);
    return 0;
}
