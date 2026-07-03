// cap09/05-grid-portals-01-bfs.cpp
// Solved exercise: Shortest Path in a Grid with Portals
//
// Given an n x m grid with free cells ('.') and blocked cells ('#'),
// plus portal pairs (a,b): entering a teleports to b with cost 0.
// Find the minimum number of steps from (0,0) to (n-1,m-1).
//
// Model: graph with weights 0 (portal) and 1 (normal move).
// Algorithm: 0-1 BFS with deque -- O(V + E) instead of Dijkstra O(E log V).
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o grid_portals 05-grid-portals-01-bfs.cpp

#include <cassert>
#include <climits>
#include <deque>
#include <string>
#include <print>
#include <vector>

using Grid = std::vector<std::string>;
using Portal = std::pair<std::pair<int,int>, std::pair<int,int>>;

// ============================================================
// 0-1 BFS: weight-0 edges go to the front of the deque,
//          weight-1 edges go to the back.
// This preserves nondecreasing distance order.
// O(V + E) -- faster than Dijkstra when weights are only 0 and 1.
// ============================================================
int bfs01(const Grid& grid, const std::vector<Portal>& portals) {
    int n = static_cast<int>(grid.size());
    int m = static_cast<int>(grid[0].size());

    auto idx = [&](int r, int c) { return r * m + c; };

    // Maps portals: position -> destination.
    std::vector<int> portal_dest(n * m, -1);
    for (const auto& [from, to] : portals) {
        portal_dest[idx(from.first, from.second)] =
            idx(to.first, to.second);
    }

    std::vector<int> dist(n * m, INT_MAX);
    std::deque<int> dq;

    int src = idx(0, 0);
    int dst = idx(n-1, m-1);
    dist[src] = 0;
    dq.push_back(src);

    constexpr int dr[] = {-1, 1, 0, 0};
    constexpr int dc[] = {0, 0, -1, 1};

    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();
        int r = u / m, c = u % m;

        if (u == dst) return dist[u];

        // Normal move: cost 1.
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (grid[nr][nc] == '#') continue;
            int v = idx(nr, nc);
            if (dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                dq.push_back(v);     // cost 1: goes to the back
            }
        }

        // Portal: cost 0.
        if (portal_dest[u] != -1) {
            int v = portal_dest[u];
            if (dist[u] < dist[v]) {
                dist[v] = dist[u];
                dq.push_front(v);    // cost 0: goes to the front
            }
        }
    }
    return dist[dst] == INT_MAX ? -1 : dist[dst];
}

int main() {
    // Simple 3x3 grid without portals.
    Grid g1 = {"...", "...", "..."};
    assert(bfs01(g1, {}) == 4);  // (0,0) to (2,2): 4 steps
    std::print("3x3 without obstacles: {} steps\n", bfs01(g1, {}));

    // With obstacle.
    Grid g2 = {"...", ".#.", "..."};
    assert(bfs01(g2, {}) == 4);  // same cost, different path
    std::print("3x3 with obstacle: {} steps\n", bfs01(g2, {}));

    // With portal: (0,1) -> (2,1) cost 0.
    // Path: (0,0)->(0,1)->[portal]->(2,1)->(2,2) = 2 steps.
    Grid g3 = {"...", "...", "..."};
    std::vector<Portal> portals = {{{{0,1},{2,1}}}};
    assert(bfs01(g3, portals) == 2);
    std::print("3x3 with portal (0,1)->(2,1): {} steps\n", bfs01(g3, portals));

    // No path.
    Grid g4 = {".#", "#."};
    assert(bfs01(g4, {}) == -1);
    std::print("blocked 2x2: {} (expected -1)\n", bfs01(g4, {}));

    std::print("05-grid-portals-01-bfs: all assertions passed\n");
    return 0;
}
