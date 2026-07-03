// cap08/01-memoization-tabulation.cpp
// Memoization (top-down) vs Tabulation (bottom-up) -- Fibonacci and Counting Paths.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o memo_tab 01-memoization-tabulation.cpp

#include <cassert>
#include <print>
#include <unordered_map>
#include <vector>

using i64 = long long;

// ============================================================
// Fibonacci -- three approaches.
// ============================================================

// Pure recursion: O(2^n) -- recomputes subproblems.
i64 fib_naive(int n) {
    if (n <= 1) return n;
    return fib_naive(n-1) + fib_naive(n-2);
}

// Memoization (top-down): O(n) -- cache in unordered_map.
i64 fib_memo(int n, std::unordered_map<int,i64>& cache) {
    if (n <= 1) return n;
    auto it = cache.find(n);
    if (it != cache.end()) return it->second;
    return cache[n] = fib_memo(n-1, cache) + fib_memo(n-2, cache);
}

// Tabulation (bottom-up): O(n) time, O(1) space (rolling).
// No recursion: fills states in increasing i order.
i64 fib_tab(int n) {
    if (n <= 1) return n;
    i64 a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        i64 c = a + b;
        a = b; b = c;
    }
    return b;
}

// ============================================================
// Counting paths in an n x m grid (only right and down moves).
// dp[r][c] = number of paths from (0,0) to (r,c)
//          = dp[r-1][c] + dp[r][c-1]
// Answer: C(n+m-2, n-1), also computable with combinatorics.
// ============================================================
i64 count_paths(int n, int m) {
    // Rolling array: only the previous row is needed -- O(m) space.
    std::vector<i64> dp(m, 1);  // first row: all 1
    for (int r = 1; r < n; ++r)
        for (int c = 1; c < m; ++c)
            dp[c] += dp[c-1];   // dp[c] = dp[r-1][c] (valor anterior), dp[c-1] = dp[r][c-1]
    return dp[m-1];
}

// ============================================================
// Counting paths with obstacles.
// grid[r][c] = 1: blocked cell.
// ============================================================
i64 count_paths_obstacles(const std::vector<std::vector<int>>& grid) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());
    std::vector<i64> dp(m, 0);
    dp[0] = (grid[0][0] == 0) ? 1 : 0;

    for (int r = 0; r < n; ++r) {
        if (grid[r][0] == 1) dp[0] = 0;
        for (int c = 1; c < m; ++c) {
            if (grid[r][c] == 1) dp[c] = 0;
            else dp[c] += dp[c-1];
        }
    }
    return dp[m-1];
}

int main() {
    // Fibonacci
    std::unordered_map<int,i64> cache;
    for (int n = 0; n <= 20; ++n) {
        i64 naive = fib_naive(n);
        i64 memo  = fib_memo(n, cache);
        i64 tab   = fib_tab(n);
        assert(naive == memo && memo == tab);
    }
    std::print("fib(20) = {}\n", fib_tab(20));
    std::print("fib(50) = {}\n", fib_tab(50));  // still fits in signed 64-bit

    // Counting paths
    assert(count_paths(3, 3) == 6);   // C(4,2)
    assert(count_paths(1, 1) == 1);
    assert(count_paths(2, 3) == 3);
    std::print("paths(3,3) = {}\n", count_paths(3, 3));

    // With obstacles
    std::vector<std::vector<int>> grid = {
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 0}
    };
    assert(count_paths_obstacles(grid) == 2);  // middle obstacle blocks 4 of the 6 paths
    std::print("paths with obstacle = {}\n", count_paths_obstacles(grid));

    std::print("01-memoization-tabulation: all assertions passed\n");
    return 0;
}
