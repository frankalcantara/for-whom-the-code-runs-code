// Exercise A3.1 - The Tandem Kayaks
// Session: A3, exercise 1 of 5 (6 hours for the evaluation)
// Theme: chapter 05 (two pointers on a sorted array; chapter 04 supplies
//   the sort)
// Tags: chapter-05, chapter-04, sorting, greedy, two-pointers
//
// MODEL: partition n weights into groups of size <= 2 with group sum
//   <= W, minimizing the number of groups.
// MATH: sort the weights. Two pointers, lightest (l) and heaviest (r):
//   if w[l] + w[r] <= W, both board one kayak (l++, r--); otherwise the
//   heaviest boards alone (r--). Each step launches one kayak.
// PROOF: exchange argument. Consider any optimal solution. The heaviest
//   paddler h sits alone or with some partner p. If w[l] + w[h] <= W,
//   pairing h with the lightest paddler l instead is feasible, and the
//   displaced parties (p, and l's former partner if any) can pair with
//   each other: their sum is at most the larger original pair sums.
//   So some optimal solution pairs h with l exactly when the greedy
//   does; if w[l] + w[h] > W, no partner fits h at all (l is lightest),
//   and h alone is forced. Induction on n.
// COMPLEXITY: O(n log n) for the sort, O(n) for the sweep. Memory O(n).
// TYPES: w_i and W fit in int but their sum w[l] + w[r] reaches 2e9 >
//   INT_MAX: the comparison is done in long long. The kayak count fits
//   in int.
// ALTERNATIVES: greedy pairing heaviest-with-heaviest fails (5,5,4,4
//   with W = 9 wants cross pairs); DP over subsets is exponential and
//   answers a question this structure already settles; multiset
//   simulation (find the best partner <= W - w with upper_bound) gives
//   the same answer in O(n log n) with more machinery.
// EDGE CASES: n = 1; everyone pairs (even n, all light); nobody pairs
//   (all > W/2); exact fits w[l] + w[r] = W; duplicate weights; the
//   guarantee w_i <= W removes the infeasible case by contract.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o kayaks a3-01-tandem-kayaks.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long cap = 0;
    std::cin >> n >> cap;

    std::vector<int> w(n);
    for (auto& x : w) std::cin >> x;

    std::ranges::sort(w);

    int boats = 0;
    int l = 0;
    int r = n - 1;
    while (l <= r) {
        if (l < r && static_cast<long long>(w[l]) + w[r] <= cap) ++l;
        --r;
        ++boats;
    }

    std::println("{}", boats);
    return 0;
}
