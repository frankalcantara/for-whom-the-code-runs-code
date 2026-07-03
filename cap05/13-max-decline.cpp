// cap05/13-max-decline.cpp
// Maximum Decline: given a sequence of stock prices, find the maximum
// price drop over any forward-looking pair (buy before sell).
//
// Problem (Amalgamated Artichokes, ICPC Regional 2014):
// Given n stock prices p[0..n-1], find max(p[i] - p[j]) for i < j.
// If prices only rise, the answer is 0.
//
// Brute force: O(n^2) — try all pairs (i,j) with i < j.
//
// Optimal O(n): scan left to right, maintaining the running maximum
// prefix seen so far.  At each j, the best decline ending at j is
//   max_prefix_before_j - p[j].
// Track the global maximum over all j.
//
// This is structurally identical to the "buy low, sell high" pattern.
//
// Complexity: O(n) time, O(1) extra space.
//
// Source: "Amalgamated Artichokes" (ICPC Regional 2014) — used here as a
// teaching example. Problem description from Phase 2, Week 1 of the
// competitive programming course (2024).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o max_decline 13-max-decline.cpp

#include <algorithm>  // std::max
#include <cassert>
#include <climits>    // LLONG_MIN
#include <print>
#include <vector>

// O(n^2) brute force — verifies the O(n) solution on small inputs.
long long max_decline_brute(const std::vector<long long>& p) {
    long long best = 0;
    int n = static_cast<int>(p.size());
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            best = std::max(best, p[i] - p[j]);
    return best;
}

// O(n) optimal: maintain max_prefix as we scan forward.
// At each position j, the best decline ending here is max_prefix - p[j].
// Update max_prefix after each step.
long long max_decline(const std::vector<long long>& p) {
    if (p.size() < 2) return 0;
    long long best       = 0;
    long long max_prefix = p[0];      // maximum price seen in p[0..j-1]
    for (int j = 1; j < static_cast<int>(p.size()); ++j) {
        best       = std::max(best, max_prefix - p[j]);
        max_prefix = std::max(max_prefix, p[j]);
    }
    return best;
}

int main() {
    struct TC { std::vector<long long> p; long long expected; };
    std::vector<TC> tests = {
        {{19, 12, 13, 11, 20, 14}, 8},   // buy at 19, sell at 11 -> drop 8
        {{19, 12, 13, 11, 20, 10}, 10},  // buy at 20, sell at 10 -> drop 10
        {{1, 2, 3, 4, 5},          0},   // only rises, no drop
        {{5, 4, 3, 2, 1},          4},   // buy at 5, sell at 1
        {{3, 3, 3},                0},   // flat
        {{100, 1, 100},            99},  // buy 100, sell 1
    };

    for (auto& [p, expected] : tests) {
        long long r_opt   = max_decline(p);
        long long r_brute = max_decline_brute(p);
        assert(r_opt   == expected);
        assert(r_brute == expected);
        std::print("max_decline(n={}) = {} OK\n", p.size(), r_opt);
    }

    std::print("13-max-decline: all asserts passed.\n");
    return 0;
}
