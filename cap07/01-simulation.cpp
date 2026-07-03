// cap07/01-simulation.cpp
// Simulation: model states and transitions explicitly.
// Problem: bank queue with k service windows.
//
// Naive: scan all service windows, O(n*k).
// Algorithmic: min-heap of (free_time, window_id), O(n log k).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o simulation 01-simulation.cpp

#include <algorithm>
#include <cassert>
#include <functional>
#include <print>
#include <queue>
#include <string>
#include <vector>

using i64 = long long;

// ============================================================
// Layer 1 -- Naive: O(n*k).
// For each customer, scans all service windows to find the earliest free one.
// ============================================================
i64 naive_bank(const std::vector<int>& times, int k) {
    std::vector<i64> free_at(k, 0);  // window i becomes free at free_at[i]
    i64 total_wait = 0;

    for (int i = 0; i < static_cast<int>(times.size()); ++i) {
        // Finds the service window with the smallest free time.
        int best = 0;
        for (int j = 1; j < k; ++j)
            if (free_at[j] < free_at[best]) best = j;

        i64 start = std::max(free_at[best], (i64)i);  // customer i arrives at time i
        total_wait += start - i;
        free_at[best] = start + times[i];
    }
    return total_wait;
}

// ============================================================
// Layer 2 -- Min-heap: O(n log k).
// Min-heap of (free_time, window_id): extracts the earliest free window.
// ============================================================
i64 heap_bank(const std::vector<int>& times, int k) {
    using P = std::pair<i64, int>;  // (free_time, id)
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    for (int j = 0; j < k; ++j) pq.push({0, j});

    i64 total_wait = 0;

    for (int i = 0; i < static_cast<int>(times.size()); ++i) {
        auto [free, id] = pq.top(); pq.pop();
        i64 start = std::max(free, (i64)i);
        total_wait += start - i;
        pq.push({start + times[i], id});
    }
    return total_wait;
}

// ============================================================
// Simulation with std::queue: BFS over states.
// Classic problem: minimum number of steps through a maze.
// ============================================================
int bfs_maze(const std::vector<std::string>& grid, int sr, int sc, int er, int ec) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, -1));
    std::queue<std::pair<int,int>> q;
    dist[sr][sc] = 0;
    q.push({sr, sc});

    constexpr int dr[] = {-1, 1, 0, 0};
    constexpr int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (r == er && c == ec) return dist[r][c];
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m
                && grid[nr][nc] != '#' && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
    return -1;  // unreachable
}

int main() {
    std::vector<int> times = {4, 2, 3, 1, 5};
    int k = 2;

    i64 naive = naive_bank(times, k);
    i64 heap  = heap_bank(times, k);
    assert(naive == heap);
    std::print("total wait ({} windows, naive=heap): {}\n", k, naive);

    // Maze BFS
    std::vector<std::string> maze = {
        "S..#.",
        ".#...",
        "...#E"
    };
    // Replaces S and E with passable cells.
    int sr=0, sc=0, er=2, ec=4;
    maze[sr][sc] = '.'; maze[er][ec] = '.';
    int steps = bfs_maze(maze, sr, sc, er, ec);
    assert(steps == 6);
    std::print("maze steps: {}\n", steps);

    std::print("01-simulation: all assertions passed\n");
    return 0;
}
