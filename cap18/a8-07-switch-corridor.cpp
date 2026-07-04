// Exercise A8.7 - The Switch Corridor
// Session: A8, exercise 7 of 10
// Theme: chapter 09 (0-1 BFS)
// Tags: chapter-09, monotonic-queue, bfs, dijkstra, zero-one-bfs
//
// MODEL: shortest path in an undirected graph with edge costs 0 or 1.
// MATH: use a deque. Relaxing a 0-edge pushes the vertex to the
//   front; relaxing a 1-edge pushes it to the back.
// PROOF: deque invariant. Vertices are processed in nondecreasing
//   distance because a 0-edge keeps the same distance and a 1-edge
//   increases it by one. This is Dijkstra specialized to weights
//   0 and 1.
// COMPLEXITY: O(n+m) time and O(n+m) memory.
// TYPES: distances are at most m, so int is enough.
// ALTERNATIVES: ordinary BFS ignores weights; Dijkstra works but is
//   O(m log n).
// EDGE CASES: unreachable target; n=1; all zero edges.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o corridor a8-07-switch-corridor.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecorridor.exe a8-07-switch-corridor.cpp

#include <deque>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, int>>> graph(n);
    for (int i = 0; i < m; ++i) {
        int u = 0, v = 0, c = 0;
        std::cin >> u >> v >> c;
        --u; --v;
        graph[u].emplace_back(v, c);
        graph[v].emplace_back(u, c);
    }
    constexpr int kInf = 1'000'000'000;
    std::vector<int> dist(n, kInf);
    std::deque<int> dq;
    dist[0] = 0;
    dq.push_front(0);
    while (!dq.empty()) {
        const int v = dq.front();
        dq.pop_front();
        for (const auto [to, cost] : graph[v]) {
            if (dist[to] > dist[v] + cost) {
                dist[to] = dist[v] + cost;
                if (cost == 0) dq.push_front(to);
                else dq.push_back(to);
            }
        }
    }
    std::print("{}\n", dist[n - 1] == kInf ? -1 : dist[n - 1]);
    return 0;
}
