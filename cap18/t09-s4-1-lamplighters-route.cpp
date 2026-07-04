// Exercise T09.5 - The Lamplighter's Route
// Session: T09.S4, exercise 1 of 3 (2 hours for the three)
// Theme: chapter 09 (0-1 BFS with a deque)
// Tags: chapter-09, sorting, monotonic-queue, bfs, dijkstra, zero-one-bfs
//
// MODEL: shortest path where every edge costs 0 (lit) or 1 (dark
//   street = one wick). The binary weights admit a specialization of
//   Dijkstra that replaces the heap with a deque: 0-1 BFS.
// MATH: relax edges as usual, but push a vertex reached through a
//   0-edge to the FRONT of the deque and through a 1-edge to the
//   BACK. The deque then always holds distances from at most two
//   consecutive values {d, d+1}, in nondecreasing order front to
//   back - the heap's invariant, maintained for free.
// PROOF: induction on pops. If the deque is sorted and spans at most
//   two distance values, the popped vertex u has the minimum pending
//   distance (front), so settling it is sound by Dijkstra's exchange
//   argument (weights nonnegative). Its relaxations create distances
//   d(u) (front-pushed, still >= every earlier front... equal in
//   fact) and d(u) + 1 (back-pushed, >= everything present), so the
//   invariant survives. A vertex may enter the deque more than once;
//   the stale-check on pop (dist changed) discards the loser, exactly
//   like lazy deletion in the heap version.
// COMPLEXITY: O(n + m) time - each edge relaxes at most twice, deque
//   operations are O(1) - and O(n + m) memory. Dijkstra's heap would
//   give O((n + m) log n): correct, and paying a log for an alphabet
//   of two weights.
// TYPES: distances are at most m <= 2e5 wicks: int everywhere. The
//   deque holds ints. After two overflow drills in this unit, an
//   exercise where int is provably enough is itself a lesson: bound
//   the answer before sizing the type.
// ALTERNATIVES: Dijkstra (works, log-heavier); BFS on a layered
//   transformation (split each 1-edge with an auxiliary vertex - same
//   asymptotics, more graph); Lex-BFS and small-weight Dial buckets
//   generalize the trick to weights {0..k}. The deque is the idiom
//   the chapter teaches because it converts insight (two distance
//   values) directly into a container choice.
// EDGE CASES: n = 1 (zero wicks, already home); home unreachable
//   (lost); an all-lit path existing however long (answer 0); the
//   greedy trap of taking a short dark route when a long lit one
//   exists (the example, reversed); parallel lit/dark streets between
//   the same corners.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o lamplighter t09-s4-1-lamplighters-route.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Felamplighter.exe t09-s4-1-lamplighters-route.cpp

#include <deque>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<std::pair<int, int>>> adj(static_cast<std::size_t>(n) + 1);
    for (int e = 0; e < m; ++e) {
        int a = 0;
        int b = 0;
        int lit = 0;
        std::cin >> a >> b >> lit;
        const int wick = 1 - lit;  // lit street costs 0 wicks, dark costs 1
        adj[static_cast<std::size_t>(a)].emplace_back(b, wick);
        adj[static_cast<std::size_t>(b)].emplace_back(a, wick);
    }

    constexpr int kUnreached = -1;
    std::vector<int> dist(static_cast<std::size_t>(n) + 1, kUnreached);
    std::deque<std::pair<int, int>> frontier;  // (cost, corner)
    dist[1] = 0;
    frontier.emplace_front(0, 1);

    while (!frontier.empty()) {
        const auto [cost, u] = frontier.front();
        frontier.pop_front();
        if (cost > dist[static_cast<std::size_t>(u)]) continue;  // stale
        for (const auto& [v, w] : adj[static_cast<std::size_t>(u)]) {
            const int candidate = cost + w;
            if (dist[static_cast<std::size_t>(v)] == kUnreached ||
                candidate < dist[static_cast<std::size_t>(v)]) {
                dist[static_cast<std::size_t>(v)] = candidate;
                if (w == 0) {
                    frontier.emplace_front(candidate, v);
                } else {
                    frontier.emplace_back(candidate, v);
                }
            }
        }
    }

    if (dist[static_cast<std::size_t>(n)] == kUnreached) {
        std::print("lost\n");
    } else {
        std::print("{}\n", dist[static_cast<std::size_t>(n)]);
    }
    return 0;
}
