// Exercise T08.1 - The Stepping Stones
// Session: T08.S1, exercise 1 of 1 (1 hour)
// Theme: chapter 08 (one-dimensional DP; rolling two variables)
// Tags: chapter-08, dynamic-programming
//
// MODEL: shortest path in a DAG whose vertices are bank, stones 1..n,
//   far bank, with edges i -> i+1 and i -> i+2 and node costs on the
//   stones. The DAG is a line, so the shortest path collapses to a
//   one-dimensional recurrence.
// MATH: let dp[i] = cheapest cost to be standing on stone i. Then
//   dp[i] = c_i + min(dp[i-1], dp[i-2]), with dp[0] = 0 for the bank
//   (and dp[-1] treated as the bank too, since the bank reaches both
//   stone 1 and stone 2). The far bank is reachable from stones n and
//   n-1, so the answer is min(dp[n], dp[n-1]); when n = 1 the bank's
//   own dp[0] = 0 also competes, because a 2-hop clears the brook.
// PROOF: induction on i. Any path standing on stone i made its last
//   hop from i-1 or i-2; both predecessors' optimal costs are already
//   final (no edges go backward), so taking the cheaper one and paying
//   c_i is optimal - the classic cut-and-paste: a cheaper path through
//   i would contain a cheaper path to one of the predecessors,
//   contradicting their optimality.
// COMPLEXITY: O(n) time, O(1) memory beyond the input - dp[i] needs
//   only the last two values, so two long long variables roll the
//   whole table. The exercise is the smallest honest instance of the
//   chapter's space-saving lesson.
// TYPES: each toll is up to 1e9 and a crossing lands on up to 2e5
//   stones, so totals reach 2e14: long long for the dp values; int
//   would overflow silently at ~2.1e9.
// ALTERNATIVES: memoized recursion computes the same values with call
//   overhead and a deeper mental stack; Dijkstra treats the line as a
//   general graph and pays a log it does not owe. Greedy (always hop
//   to the cheaper of the next two reachable stones) fails on
//   1 1 100 1: it takes stone 1, then stone 2, then stone 4, paying 3,
//   while bank -> 2 -> 4 -> far pays 2.
// EDGE CASES: n = 1 (answer 0, hop straight over); n = 2 (answer
//   min(c_1, c_2)); equal tolls everywhere (any maximal-hop path);
//   maximum n with maximum tolls (overflow drill).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o stones t08-s1-1-stepping-stones.cpp

#include <algorithm>
#include <iostream>
#include <print>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    long long two_back = 0;   // dp[i-2], starts as the bank
    long long one_back = 0;   // dp[i-1], starts as the bank
    for (int i = 1; i <= n; ++i) {
        long long c = 0;
        std::cin >> c;
        const long long here = c + std::min(one_back, two_back);
        two_back = one_back;
        one_back = here;
    }
    // one_back = dp[n], two_back = dp[n-1] (or the bank when n == 1).
    std::print("{}\n", std::min(one_back, two_back));
    return 0;
}
