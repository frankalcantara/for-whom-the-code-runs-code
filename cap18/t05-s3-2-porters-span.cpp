// Exercise T05.4 - The Porter's Span
// Session: T05.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 05 (two pointers; longest window under a sum budget)
// Tags: chapter-05, two-pointers, sliding-window
//
// MODEL: longest contiguous window of w[0..n-1] with sum <= W, all
//   weights strictly positive.
// MATH: slide a window [l, r]. Extending r increases the sum; shrinking
//   from l decreases it, both strictly, because every w_i >= 1. For each
//   r, advance l just until the sum fits; the candidate length is
//   r - l + 1. Both pointers only move forward: 2n pointer steps total.
// PROOF: monotonicity. Let L(r) be the smallest l such that sum(l..r)
//   <= W. Since all weights are positive, sum(l..r+1) > sum(l..r), so
//   L(r+1) >= L(r): the optimal left boundary never moves back, and the
//   forward-only sweep visits every (r, L(r)) pair. Every window not
//   visited is contained in a visited one and therefore no longer.
// COMPLEXITY: O(n) time, O(1) extra memory (the weights need not even be
//   stored; the window sum is maintained as crates enter and leave...
//   this program stores them, because the leaving crate's weight must be
//   known, so O(n) memory in total).
// TYPES: window sums reach 1e6 * 1e9 = 1e15 and W reaches 1e18: both in
//   long long. The comparison sum <= W never overflows because the sum is
//   capped by the running shrink before the next crate enters.
// ALTERNATIVES: binary search over the answer length with prefix sums,
//   O(n log n): correct, slower, and it spends an array of prefix sums to
//   rediscover a monotonicity the two pointers exploit directly. With
//   negative weights two pointers would be invalid and the prefix-sum
//   approach (with a monotonic structure) would be required.
// EDGE CASES: no crate fits (every w_i > W, answer 0); every crate fits
//   in one barrow (answer n); single crate exactly equal to W; n = 1
//   both fitting and not; the 1e15 full sum against W = 1e18.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o porter t05-s3-2-porters-span.cpp

#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long cap = 0;
    std::cin >> n >> cap;

    std::vector<long long> w(n);
    for (auto& x : w) std::cin >> x;

    long long sum = 0;
    int best = 0;
    int l = 0;
    for (int r = 0; r < n; ++r) {
        sum += w[r];
        while (sum > cap && l <= r) sum -= w[l++];
        if (sum <= cap && r - l + 1 > best) best = r - l + 1;
    }

    std::println("{}", best);
    return 0;
}
