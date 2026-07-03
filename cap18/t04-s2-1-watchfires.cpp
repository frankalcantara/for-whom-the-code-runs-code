// Exercise T04.2 - The Watchfires
// Session: T04.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 04 (binary search on answers, maximization form, greedy
//   feasibility check)
// Tags: chapter-04, binary-search-answer, binary-search, sorting, greedy
//
// MODEL: choose k of n positions maximizing the minimum pairwise distance.
// MATH: define feasible(d) = "k fires can be placed with every adjacent
//   pair at distance >= d". The predicate is monotone: shrinking d only
//   relaxes the constraint, so feasibility forms the pattern
//   true...true false...false over d. The answer is the last true, found
//   by the maximization form of binary search with the upward-rounding
//   midpoint (without the +1 in mid, the loop can stop making progress and
//   becomes immortal).
// PROOF: (monotonicity) any placement valid for d is valid for d' < d.
//   (greedy check) sort positions; light the first outcrop, then always
//   the first outcrop at distance >= d from the last lit one. Exchange
//   argument: any valid placement's i-th fire can be moved left to the
//   greedy's i-th choice without breaking the spacing, so greedy lights
//   the maximum number of fires for that d; if greedy cannot reach k,
//   nothing can. Binary search on a monotone predicate then returns the
//   exact optimum.
// COMPLEXITY: sort O(n log n); each check O(n); range up to 1e18 gives
//   ~60 checks: O(n log(range)) total. O(n) memory.
// TYPES: positions and distances reach 1e18: long long everywhere; the
//   midpoint is computed as lo + (hi - lo + 1) / 2, which cannot overflow
//   where (lo + hi) / 2 could.
// ALTERNATIVES: trying all C(n, k) subsets is combinatorial fiction; for
//   k = 2 the answer is simply the maximum gap, max - min, a special case
//   worth noticing before reaching for machinery.
// EDGE CASES: k = n (every fire lit, answer is the minimum adjacent gap);
//   k = 2 (answer is max - min); duplicate positions (distance 0 pairs,
//   feasible only when k does not exceed distinct positions... d = 0 is
//   always feasible and the search floor); already-sorted input.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o watchfires t04-s2-1-watchfires.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

bool feasible(const std::vector<long long>& p, int k, long long d) {
    int lit = 1;
    long long last = p[0];
    for (std::size_t i = 1; i < p.size() && lit < k; ++i) {
        if (p[i] - last >= d) {
            ++lit;
            last = p[i];
        }
    }
    return lit >= k;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int k = 0;
    std::cin >> n >> k;

    std::vector<long long> p(n);
    for (auto& x : p) std::cin >> x;
    std::ranges::sort(p);

    long long lo = 0;                 // always feasible
    long long hi = p[n - 1] - p[0];   // no spacing can exceed the span
    while (lo < hi) {
        long long mid = lo + (hi - lo + 1) / 2;  // round up: last true
        if (feasible(p, k, mid)) lo = mid;
        else                     hi = mid - 1;
    }

    std::print("{}\n", lo);
    return 0;
}
