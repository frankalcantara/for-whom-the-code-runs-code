// cap11/02-counting-dp.cpp
// Counting with dynamic programming: subset sums, grid paths, and constrained binary strings.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o counting_dp 02-counting-dp.cpp

#include <array>
#include <cassert>
#include <print>
#include <vector>

using i64 = long long;
constexpr i64 MOD = 1000000007;

// Count subsets with target sum.
// Descending sum order guarantees each input element is used at most once.
i64 count_subsets_sum(const std::vector<int>& a, int target) {
    std::vector<i64> dp(target + 1, 0);
    dp[0] = 1;
    for (int x : a)
        for (int j = target; j >= x; --j)
            dp[j] = (dp[j] + dp[j - x]) % MOD;
    return dp[target];
}

// Count right/down grid paths with blocked cells.
// grid[r][c] == 1 means the cell is blocked.
i64 count_paths(const std::vector<std::vector<int>>& grid) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());
    std::vector<std::vector<i64>> dp(n, std::vector<i64>(m, 0));
    if (grid[0][0] == 1) return 0;
    dp[0][0] = 1;
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < m; ++c) {
            if (r == 0 && c == 0) continue;
            if (grid[r][c] == 1) { dp[r][c] = 0; continue; }
            if (r > 0) dp[r][c] = (dp[r][c] + dp[r - 1][c]) % MOD;
            if (c > 0) dp[r][c] = (dp[r][c] + dp[r][c - 1]) % MOD;
        }
    return dp[n - 1][m - 1];
}

// Count binary strings of length n with exactly k ones and no adjacent ones.
i64 count_binary_no_adj(int n, int k) {
    std::vector<std::array<i64, 2>> dp(k + 1, {0, 0});
    dp[0][0] = 1;
    for (int i = 0; i < n; ++i) {
        std::vector<std::array<i64, 2>> ndp(k + 1, {0, 0});
        for (int j = 0; j <= k; ++j) {
            ndp[j][0] = (ndp[j][0] + dp[j][0] + dp[j][1]) % MOD;
            if (j + 1 <= k)
                ndp[j + 1][1] = (ndp[j + 1][1] + dp[j][0]) % MOD;
        }
        dp = ndp;
    }
    return (dp[k][0] + dp[k][1]) % MOD;
}

int main() {
    assert(count_subsets_sum({1, 2, 3, 4}, 4) == 2); // {4}, {1,3}
    assert(count_subsets_sum({1, 1, 1, 1}, 2) == 6); // choose two positions

    assert(count_paths({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}) == 6);
    assert(count_paths({{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}) == 2);
    assert(count_paths({{1, 0}, {0, 0}}) == 0);

    assert(count_binary_no_adj(4, 2) == 3); // 1010, 1001, 0101
    assert(count_binary_no_adj(5, 2) == 6);
    assert(count_binary_no_adj(3, 1) == 3);

    std::print("02-counting-dp: all assertions passed\n");
    return 0;
}
