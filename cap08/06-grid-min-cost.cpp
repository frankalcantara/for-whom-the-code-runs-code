// cap08/06-grid-min-cost.cpp
// Solved exercise: Minimum-Cost Grid + Number of Optimal Paths
//
// Given an n x m grid with integer costs, find the minimum-cost path
// from (0,0) to (n-1,m-1), moving only right or down.
// Also count the number of minimum-cost paths modulo 10^9+7.
//
// dp_cost[r][c] = minimum cost to (r,c)
// dp_cnt[r][c]  = number of minimum-cost paths to (r,c)
//
// Rolling array: O(m) space for each DP field.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o grid_min_cost 06-grid-min-cost.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fegrid_min_cost.exe 06-grid-min-cost.cpp

#include <cassert>
#include <climits>
#include <print>
#include <vector>

using i64 = long long;
constexpr i64 MOD = 1e9 + 7;

struct GridResult { i64 min_cost; i64 num_paths; };

GridResult grid_dp(const std::vector<std::vector<int>>& grid) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());

    // Minimum cost: row rolling array.
    std::vector<i64> cost(m, LLONG_MAX / 2);
    std::vector<i64> cnt(m, 0);

    cost[0] = grid[0][0];
    cnt[0]  = 1;

    // First row: only right moves.
    for (int c = 1; c < m; ++c) {
        cost[c] = cost[c-1] + grid[0][c];
        cnt[c]  = 1;
    }

    for (int r = 1; r < n; ++r) {
        // First column: only down moves.
        cost[0] += grid[r][0];
        // cnt[0] permanece 1

        for (int c = 1; c < m; ++c) {
            i64 from_top  = cost[c]   + grid[r][c];  // from (r-1,c)
            i64 from_left = cost[c-1] + grid[r][c];  // from (r,c-1)

            if (from_top < from_left) {
                cost[c] = from_top;
                cnt[c]  = cnt[c];        // only paths from the top
            } else if (from_left < from_top) {
                cost[c] = from_left;
                cnt[c]  = cnt[c-1];      // only paths from the left
            } else {
                cost[c] = from_top;
                cnt[c]  = (cnt[c] + cnt[c-1]) % MOD;  // both are optimal
            }
        }
    }

    return {cost[m-1], cnt[m-1]};
}

// Naive verification: O(2^(n+m)).
void naive_grid(const std::vector<std::vector<int>>& grid,
                int r, int c, i64 cur,
                i64& best, i64& cnt) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());
    cur += grid[r][c];
    if (r == n-1 && c == m-1) {
        if (cur < best)       { best = cur; cnt = 1; }
        else if (cur == best) ++cnt;
        return;
    }
    if (r+1 < n) naive_grid(grid, r+1, c, cur, best, cnt);
    if (c+1 < m) naive_grid(grid, r, c+1, cur, best, cnt);
}

int main() {
    // 3x3 grid
    std::vector<std::vector<int>> g = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };

    auto [cost, paths] = grid_dp(g);
    assert(cost == 7);
    assert(paths == 1);
    std::print("grid 3x3: cost={}, paths={}\n", cost, paths);

    // Verifies with naive recursion.
    i64 naive_best = LLONG_MAX / 2, naive_cnt = 0;
    naive_grid(g, 0, 0, 0, naive_best, naive_cnt);
    assert(cost == naive_best && paths == naive_cnt);

    // Grid with equal costs -- multiple minimum paths.
    std::vector<std::vector<int>> g2 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    // All paths cost 5 (4 moves + initial cell); paths = C(4,2) = 6.
    auto [c2, p2] = grid_dp(g2);
    assert(c2 == 5);
    assert(p2 == 6);
    std::print("uniform 3x3 grid: cost={}, paths={}\n", c2, p2);

    i64 b2 = LLONG_MAX/2, n2 = 0;
    naive_grid(g2, 0, 0, 0, b2, n2);
    assert(c2 == b2 && p2 == n2);

    std::print("06-grid-min-cost: all assertions passed\n");
    return 0;
}
