// Exercise T08.3 - The Tram Fare
// Session: T08.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 08 (coin change, minimization variant)
// Tags: chapter-08, greedy, bfs, dynamic-programming
//
// MODEL: unbounded coin change, minimum-count variant: shortest way to
//   write f as a sum of denominations, repetition allowed. Equivalently
//   a BFS distance in the graph of amounts - which is why DP and BFS
//   give the same number; the DP is the BFS with the queue optimized
//   away by the natural order of amounts.
// MATH: dp[a] = minimum coins summing exactly to a; dp[0] = 0 and
//   dp[a] = 1 + min over d <= a of dp[a - d], infinity where no option
//   exists. Computing a ascending makes every dp[a - d] final before
//   it is read.
// PROOF: induction on a. Any optimal multiset for a has some last coin
//   d; removing it leaves an optimal multiset for a - d (cut and
//   paste: a better one would improve a as well). The recurrence takes
//   the min over every possible last coin, which is exhaustive. The
//   greedy counterexample lives in the example: 24 with {1, 5, 7} -
//   greedy takes 7 7 7 and must finish with 1 1 1, six coins, while
//   the optimum 7 7 5 5 uses four.
// COMPLEXITY: O(nf) time (1e7 at the limits), O(f) memory. The
//   "infinity" sentinel must not overflow when 1 is added: f + 1 is a
//   safe infinity, since no exact sum uses more than f coins (every
//   denomination is >= 1).
// TYPES: counts are at most f <= 1e5, so int holds dp values, with
//   the sentinel f + 1 well inside range; no long long needed - and
//   noting WHY (bounded by f, not by 1e9 values) is the type lesson.
// ALTERNATIVES: BFS over amounts finds the same distances with a queue
//   it does not need; greedy by largest denomination works for some
//   coin systems (canonical ones - most national coinages) but not in
//   general, and competitive judges are not national mints.
// EDGE CASES: f itself a denomination (answer 1); only even
//   denominations with odd f (impossible); denominations larger than f
//   (ignored by the d <= a guard); duplicate denominations (harmless);
//   f = 1 with and without a 1-coin.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o fare t08-s3-1-tram-fare.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fefare.exe t08-s3-1-tram-fare.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int fare = 0;
    std::cin >> n >> fare;

    std::vector<int> denom(static_cast<std::size_t>(n));
    for (auto& d : denom) {
        std::cin >> d;
    }

    const int kInfinity = fare + 1;
    std::vector<int> dp(static_cast<std::size_t>(fare) + 1, kInfinity);
    dp[0] = 0;
    for (int a = 1; a <= fare; ++a) {
        for (const int d : denom) {
            if (d <= a) {
                dp[static_cast<std::size_t>(a)] =
                    std::min(dp[static_cast<std::size_t>(a)],
                             dp[static_cast<std::size_t>(a - d)] + 1);
            }
        }
    }

    if (dp[static_cast<std::size_t>(fare)] >= kInfinity) {
        std::print("impossible\n");
    } else {
        std::print("{}\n", dp[static_cast<std::size_t>(fare)]);
    }
    return 0;
}
