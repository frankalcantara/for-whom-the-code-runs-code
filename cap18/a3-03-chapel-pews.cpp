// Exercise A3.3 - The Chapel Pews
// Session: A3, exercise 3 of 5 (6 hours for the evaluation)
// Theme: chapter 02 revision (prefix sums; immutable array, many range
//   sums)
// Tags: chapter-02, prefix-sum
//
// MODEL: q range-sum queries over an array that never changes. The
//   statement says "the amounts never change" — that sentence is the
//   entire data-structure decision.
// MATH: P[i] = a[1] + ... + a[i], P[0] = 0. Then
//   sum(l, r) = P[r] - P[l-1]: one subtraction per audit.
// PROOF: telescoping. P[r] counts every pledge up to r, P[l-1] removes
//   every pledge before l; what remains is exactly [l, r]. Induction on
//   r if one insists, but the cancellation is the whole argument.
// COMPLEXITY: O(n) build, O(1) per audit: O(n + q) total, O(n) memory.
//   Summing per audit is O(nq) = 4e10. A Fenwick tree (chapter 06,
//   fresh in the mind during this evaluation) also answers in O(log n) —
//   and is the wrong tool, because it buys update support this statement
//   explicitly rules out. Recognizing absent requirements is revision
//   too.
// TYPES: total pledges reach 2e5 * 1e9 = 2e14: long long for P. The
//   inputs fit in int; the prefix array does not.
// ALTERNATIVES: sparse table answers range sums only via heavier
//   machinery (sums are not idempotent, so it would need disjoint
//   blocks); sqrt decomposition is for updates that never come. The
//   prefix array is fifteen minutes of work, which is the point of a
//   warm review slot in a six-hour paper.
// EDGE CASES: l = 1 (P[0] = 0 must exist); l = r; the whole row;
//   pledges of 0 (a zero answer is legal); the 2e14 total.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o pews a3-03-chapel-pews.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fepews.exe a3-03-chapel-pews.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        long long a = 0;
        std::cin >> a;
        prefix[i] = prefix[i - 1] + a;
    }

    while (q-- > 0) {
        int l = 0;
        int r = 0;
        std::cin >> l >> r;
        std::print("{}\n", prefix[r] - prefix[l - 1]);
    }
    return 0;
}
