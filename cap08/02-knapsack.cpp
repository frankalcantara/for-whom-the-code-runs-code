// cap08/02-knapsack.cpp
// 0/1 Knapsack, unbounded knapsack, and Subset Sum with DP.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o knapsack 02-knapsack.cpp

#include <cassert>
#include <algorithm>
#include <numeric>
#include <print>
#include <vector>

using i64 = long long;

// ============================================================
// 0/1 Knapsack: each item can be used 0 or 1 time.
// dp[c] = maximum value with capacity c.
// Iteration scans c in decreasing order so each item is used at most once.
// O(n * W) time, O(W) space (1D rolling table).
// ============================================================
i64 knapsack01(const std::vector<int>& weight,
               const std::vector<int>& value, int W) {
    int n = static_cast<int>(weight.size());
    std::vector<i64> dp(W + 1, 0);

    for (int i = 0; i < n; ++i)
        for (int c = W; c >= weight[i]; --c)   // decreasing: no reuse
            dp[c] = std::max(dp[c], dp[c - weight[i]] + value[i]);

    return dp[W];
}

// ============================================================
// Unbounded knapsack: each item can be used any number of times.
// Iteration scans c in increasing order to allow reuse.
// ============================================================
i64 knapsack_unbounded(const std::vector<int>& weight,
                        const std::vector<int>& value, int W) {
    int n = static_cast<int>(weight.size());
    std::vector<i64> dp(W + 1, 0);

    for (int c = 1; c <= W; ++c)
        for (int i = 0; i < n; ++i)
            if (weight[i] <= c)
                dp[c] = std::max(dp[c], dp[c - weight[i]] + value[i]);

    return dp[W];
}

// ============================================================
// Subset Sum: is there a subset with sum exactly T?
// dp[s] = true if sum s is reachable.
// 0/1 knapsack with value = 1 and weight = element.
// O(n * T) time, O(T) space.
// ============================================================
bool subset_sum(const std::vector<int>& a, int T) {
    std::vector<char> dp(T + 1, false);
    dp[0] = true;

    for (int x : a)
        for (int s = T; s >= x; --s)   // decreasing: 0/1
            if (dp[s - x]) dp[s] = true;

    return dp[T];
}

// ============================================================
// Partition Equal Subset Sum
// Is there a partition of a[] into two subsets with equal sum?
// Equivalent to: is there a subset with sum = total/2?
// ============================================================
bool partition_equal(const std::vector<int>& a) {
    int total = std::accumulate(a.begin(), a.end(), 0);
    if (total % 2 != 0) return false;
    return subset_sum(a, total / 2);
}

int main() {
    // Knapsack 0/1
    std::vector<int> w = {1, 3, 4, 5};
    std::vector<int> v = {1, 4, 5, 7};
    assert(knapsack01(w, v, 7) == 9);   // items 2+3: weight 3+4=7, value 4+5=9
    assert(knapsack01(w, v, 0) == 0);
    std::print("knapsack01(W=7) = {}\n", knapsack01(w, v, 7));

    // Unbounded knapsack
    std::vector<int> wu = {1, 3, 4};
    std::vector<int> vu = {2, 6, 8};
    // W=7: weight 3 twice + weight 1 once = value 14, or weight 4+3 = value 14.
    assert(knapsack_unbounded(wu, vu, 7) == 14);
    std::print("knapsack_unbounded(W=7) = {}\n", knapsack_unbounded(wu, vu, 7));

    // Subset Sum
    assert(subset_sum({1,5,11,5}, 11) == true);
    assert(subset_sum({1,5,3},    7)  == false);
    std::print("subset_sum({{1,5,11,5}}, 11) = {}\n", subset_sum({1,5,11,5}, 11));

    // Partition
    assert(partition_equal({1,5,11,5}) == true);   // {11} and {1,5,5}
    assert(partition_equal({1,2,3,5})  == false);
    std::print("partition_equal({{1,5,11,5}}) = {}\n", partition_equal({1,5,11,5}));

    std::print("02-knapsack: all assertions passed\n");
    return 0;
}
