// cap08/07-board-game.cpp
// Board Game Grid DP: minimize the score reaching the bottom-right corner
// of an N x N grid, moving only right or down.
//
// Problem (CodeVita, Phase 1): given an N x N grid where every cell (i,j)
// except the top-left holds a positive integer, start at (0,0) with score 0.
// When moving to a new cell, the score updates as:
//   new_score = floor(current_score / 2) + grid[i][j]
// Find the minimum possible score at cell (N-1, N-1).
//
// State: dp[i][j] = minimum score to reach (i,j).
//
// Recurrence:
//   dp[0][0] = 0
//   dp[0][j] = floor(dp[0][j-1] / 2) + grid[0][j]       (first row, only from left)
//   dp[i][0] = floor(dp[i-1][0] / 2) + grid[i][0]       (first column, only from above)
//   dp[i][j] = min(floor(dp[i-1][j] / 2), floor(dp[i][j-1] / 2)) + grid[i][j]
//
// Note: floor(x/2) for integer x is simply x >> 1 (arithmetic right shift).
//
// Complexity: O(N^2) time and space.
//
// Source: CodeVita competitive programming contest, Phase 1 (2024).
//   Original C++ solution by the course team, rewritten in C++23.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o board_game 07-board-game.cpp

#include <algorithm>   // std::min
#include <cassert>
#include <print>
#include <ranges>
#include <span>
#include <vector>

// Solve using a full 2D dp table.
// grid[0][0] is the starting cell (score 0, not added to the score).
int board_game(std::span<const std::vector<int>> grid) {
    int N = static_cast<int>(grid.size());
    std::vector<std::vector<int>> dp(N, std::vector<int>(N, 0));

    // First row: only reachable from the left.
    for (int j : std::views::iota(1, N))
        dp[0][j] = dp[0][j-1] / 2 + grid[0][j];

    // First column: only reachable from above.
    for (int i : std::views::iota(1, N))
        dp[i][0] = dp[i-1][0] / 2 + grid[i][0];

    // Interior: choose the predecessor with lower score.
    for (int i : std::views::iota(1, N))
        for (int j : std::views::iota(1, N))
            dp[i][j] = std::min(dp[i-1][j] / 2, dp[i][j-1] / 2) + grid[i][j];

    return dp[N-1][N-1];
}

int main() {
    // 2x2: both paths give 5.
    //   right-down: 0/2+3=3, 3/2+4=5
    //   down-right: 0/2+2=2, 2/2+4=5
    {
        std::vector<std::vector<int>> g = {{0, 3}, {2, 4}};
        assert(board_game(g) == 5);
        std::print("2x2: {}\n", board_game(g));
    }

    // 3x3 example: grid below, computed manually or by brute.
    //   [[0, 4, 8],
    //    [3, 2, 5],
    //    [5, 3, 7]]
    //
    //   dp[0]: [0, 4, 6]   (0/2+4=4, 4/2+8=10... wait: 4/2=2, +8=10)
    //   Let me recompute:
    //   dp[0][1] = 0/2 + 4 = 4
    //   dp[0][2] = 4/2 + 8 = 2 + 8 = 10
    //   dp[1][0] = 0/2 + 3 = 3
    //   dp[2][0] = 3/2 + 5 = 1 + 5 = 6
    //   dp[1][1] = min(dp[0][1]/2, dp[1][0]/2) + 2 = min(2, 1) + 2 = 3
    //   dp[1][2] = min(dp[0][2]/2, dp[1][1]/2) + 5 = min(5, 1) + 5 = 6
    //   dp[2][1] = min(dp[1][1]/2, dp[2][0]/2) + 3 = min(1, 3) + 3 = 4
    //   dp[2][2] = min(dp[1][2]/2, dp[2][1]/2) + 7 = min(3, 2) + 7 = 9
    {
        std::vector<std::vector<int>> g = {{0,4,8},{3,2,5},{5,3,7}};
        assert(board_game(g) == 9);
        std::print("3x3: {}\n", board_game(g));
    }

    // 1x1: trivially 0 (already at destination, score never updated).
    {
        std::vector<std::vector<int>> g = {{0}};
        assert(board_game(g) == 0);
    }

    std::print("07-board-game: all asserts passed.\n");
    return 0;
}
