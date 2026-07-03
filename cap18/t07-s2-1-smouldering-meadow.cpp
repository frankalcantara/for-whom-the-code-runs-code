// Exercise T07.2 - The Smouldering Meadow
// Session: T07.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 07 (BFS as simulation, multi-source)
// Tags: chapter-07, bfs, dynamic-programming, simulation
//
// MODEL: the meadow is an unweighted grid graph; burning is a wavefront
//   that advances one edge per minute from every burning cell at once.
//   The minute a cell catches fire is its BFS distance to the NEAREST
//   smouldering patch, which is exactly what a multi-source BFS computes.
// MATH: let d(v) = min over sources s of dist(s, v). Seeding the queue
//   with all sources at distance 0 computes d directly: the queue
//   processes cells in nondecreasing d, so each cell is finalized the
//   first time it is reached. The simulation and the shortest-path view
//   are the same computation read twice.
// PROOF: induction on the wavefront. Invariant: when the queue's front
//   has distance k, every cell with d < k has been popped with its
//   correct d, and the queue holds exactly the cells with d = k or
//   k + 1, correctly labeled. Each neighbor relaxation extends the
//   invariant; the base case is the seeded sources at d = 0. The fire
//   simulation obeys the same recurrence (a cell burns one minute after
//   its earliest-burning neighbor), so d(barn) is the answer.
// COMPLEXITY: O(rc) time and memory; each cell enters the queue at most
//   once and each of the <= 4rc edges is examined once. For 1000 x 1000
//   that is 1e6 cells, comfortably within limits.
// TYPES: distances are at most rc <= 1e6, so int holds them. Cells are
//   indexed as r * c + col in a flat vector<int> to keep the visited
//   array cache-friendly (Chapter 1's locality lesson applied).
// ALTERNATIVES: simulating minute by minute with two buffers ("current
//   front", "next front") is the same algorithm wearing work clothes;
//   Dijkstra solves it too but pays an unnecessary log factor for unit
//   weights. DFS is wrong: it explores depth-first and does not produce
//   earliest burn times.
// EDGE CASES: barn adjacent to an ember (answer 1); barn walled off by
//   rock (safe); meadow entirely rock except B and * (safe); 1 x 1 is
//   impossible by the input contract (B and * both exist), but 1 x 2
//   "*B" works (answer 1); fire that must snake around a rock spiral.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o meadow t07-s2-1-smouldering-meadow.cpp

#include <iostream>
#include <print>
#include <queue>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int r = 0;
    int c = 0;
    std::cin >> r >> c;

    std::vector<std::string> grid(r);
    for (auto& row : grid) {
        std::cin >> row;
    }

    constexpr int kUnreached = -1;
    std::vector<int> minute(static_cast<std::size_t>(r) * c, kUnreached);
    std::queue<int> frontier;

    int barn = -1;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            const int id = i * c + j;
            if (grid[i][j] == '*') {
                minute[id] = 0;
                frontier.push(id);
            } else if (grid[i][j] == 'B') {
                barn = id;
            }
        }
    }

    const int dr[] = {-1, 1, 0, 0};
    const int dc[] = {0, 0, -1, 1};
    while (!frontier.empty()) {
        const int id = frontier.front();
        frontier.pop();
        const int i = id / c;
        const int j = id % c;
        for (int k = 0; k < 4; ++k) {
            const int ni = i + dr[k];
            const int nj = j + dc[k];
            if (ni < 0 || ni >= r || nj < 0 || nj >= c) continue;
            if (grid[ni][nj] == '#') continue;
            const int nid = ni * c + nj;
            if (minute[nid] != kUnreached) continue;
            minute[nid] = minute[id] + 1;
            frontier.push(nid);
        }
    }

    if (minute[barn] == kUnreached) {
        std::print("safe\n");
    } else {
        std::print("{}\n", minute[barn]);
    }
    return 0;
}
