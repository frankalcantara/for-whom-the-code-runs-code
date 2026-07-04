// Exercise T02.4 - The Bakery Shelf
// Session: T02.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 02 (fixed-length range sums via prefix sums)
// Tags: chapter-02, prefix-sum, sliding-window
//
// MODEL: maximize sum(i..i+k-1) over all 0-based starts i in [0, n-k].
// MATH: with prefixes p[0..n], the window starting at i has sum
//   p[i+k] - p[i]. There are n-k+1 candidate windows; evaluating each is
//   one subtraction once the prefixes exist.
// PROOF: the prefix identity (induction, as in T02.1) makes every window
//   sum exact; taking the maximum over all starts examines every candidate
//   exactly once, so the reported value is the global maximum.
// COMPLEXITY: O(n) time, O(n) memory for the prefix array.
// TYPES: warmth is signed and reaches |1e9| over up to 2e5 loaves, so
//   window sums reach |2e14|: long long. The maximum must be initialized
//   with the first window's value, not with zero; an all-negative shelf
//   would otherwise report a photograph of nothing, and the guide insists
//   on photographing actual bread.
// ALTERNATIVES: recomputing each window costs O(nk); a sliding update
//   (add the entering loaf, drop the leaving one) is also O(n) with O(1)
//   memory and appears formally in Chapter 5. Prefix sums solve it with
//   Chapter 2 vocabulary at the same asymptotic price.
// EDGE CASES: k == n (single window); k == 1 (the answer is the maximum
//   element); all warmths negative; ties between windows.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o bakery t02-s3-2-bakery-shelf.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febakery.exe t02-s3-2-bakery-shelf.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        long long warmth = 0;
        std::cin >> warmth;
        prefix[i + 1] = prefix[i] + warmth;
    }

    long long best = prefix[k] - prefix[0];
    for (int start = 1; start + k <= n; ++start) {
        long long window = prefix[start + k] - prefix[start];
        if (window > best) best = window;
    }

    std::print("{}\n", best);
    return 0;
}
