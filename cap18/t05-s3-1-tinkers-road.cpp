// Exercise T05.3 - The Tinker's Road
// Session: T05.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 05 (Kadane; maximum subarray with mandatory nonempty pick)
// Tags: chapter-05, greedy, dynamic-programming
//
// MODEL: maximum-sum contiguous subarray of p[0..n-1], length >= 1.
// MATH: let best_end(i) be the maximum sum of a subarray ending exactly
//   at i. Then best_end(i) = p[i] + max(0, best_end(i-1)): either the
//   stretch ending at i-1 helps (is positive) and is extended, or it is
//   dropped and the stretch restarts at i. The answer is max over i of
//   best_end(i).
// PROOF: exchange argument inside the recurrence. Any subarray ending at
//   i is p[i] plus some subarray ending at i-1 or nothing; replacing that
//   prefix by the best one ending at i-1 (or by nothing, if the best is
//   negative) never decreases the sum, so the recurrence considers an
//   optimal candidate for every i, and the global maximum is over all
//   possible right endpoints.
// COMPLEXITY: O(n) time, O(1) memory beyond the input. Prefix sums with
//   a running minimum are also O(n); the all-pairs scan is O(n^2) = 1e12.
// TYPES: a full positive road sums to 1e6 * 1e9 = 1e15, and a full
//   negative road to -1e15: long long on both accumulators. Initializing
//   the answer with the first element (not with 0) is what makes the
//   all-negative road return the least bad village instead of an illegal
//   empty stretch.
// ALTERNATIVES: divide and conquer in O(n log n) (the segment-tree-ready
//   form); prefix sums minus running minimum prefix, same O(n) at the
//   cost of one extra pass. Kadane wins on simplicity and on memory.
// EDGE CASES: n = 1; all profits negative (answer is the maximum single
//   element); all positive (answer is the whole road); the 1e15 sum at
//   both signs; zeros mixed with negatives.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o tinker t05-s3-1-tinkers-road.cpp

#include <algorithm>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    long long best = 0;
    long long ending_here = 0;
    for (int i = 0; i < n; ++i) {
        long long p = 0;
        std::cin >> p;
        ending_here = p + std::max(0LL, ending_here);
        if (i == 0 || ending_here > best) best = ending_here;
    }

    std::println("{}", best);
    return 0;
}
