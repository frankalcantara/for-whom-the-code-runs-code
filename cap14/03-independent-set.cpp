// cap14/03-independent-set.cpp
// Solved exercise: maximum independent set for small graphs.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o independent_set 03-independent-set.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feindependent_set.exe 03-independent-set.cpp

#include <algorithm>
#include <cassert>
#include <bit>
#include <print>
#include <vector>

int max_independent_set(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<unsigned> adj(n, 0);
    for (auto [u, v] : edges) {
        adj[u] |= (1u << v);
        adj[v] |= (1u << u);
    }

    int best = 0;
    for (unsigned mask = 0; mask < (1u << n); ++mask) {
        bool independent = true;
        for (int u = 0; u < n && independent; ++u) {
            if (!(mask & (1u << u))) continue;
            if (adj[u] & mask) independent = false;
        }
        if (independent)
            best = std::max(best, static_cast<int>(std::popcount(mask)));
    }
    return best;
}

struct BipartiteMatching {
    int n, m;
    std::vector<std::vector<int>> adj;
    std::vector<int> match_l, match_r;
    std::vector<bool> vis;

    BipartiteMatching(int n, int m)
        : n(n), m(m), adj(n), match_l(n, -1), match_r(m, -1), vis(m) {}

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (vis[v]) continue;
            vis[v] = true;
            if (match_r[v] == -1 || dfs(match_r[v])) {
                match_l[u] = v;
                match_r[v] = u;
                return true;
            }
        }
        return false;
    }

    int max_matching() {
        match_l.assign(n, -1);
        match_r.assign(m, -1);
        int res = 0;
        for (int u = 0; u < n; ++u) {
            vis.assign(m, false);
            if (dfs(u)) ++res;
        }
        return res;
    }
};

int main() {
    assert(max_independent_set(4, {{0, 1}, {1, 2}, {2, 3}}) == 2);
    assert(max_independent_set(4, {{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}) == 1);
    assert(max_independent_set(5, {}) == 5);

    {
        BipartiteMatching bm(2, 3);
        bm.add_edge(0, 0); bm.add_edge(0, 1); bm.add_edge(0, 2);
        bm.add_edge(1, 0); bm.add_edge(1, 1); bm.add_edge(1, 2);
        int mm = bm.max_matching();
        assert(mm == 2);
        assert(2 + 3 - mm == 3);
    }

    {
        BipartiteMatching bm(3, 2);
        bm.add_edge(0, 0); bm.add_edge(1, 0); bm.add_edge(2, 1);
        int mm = bm.max_matching();
        assert(mm == 2);
        assert(3 + 2 - mm == 3);
    }

    std::print("03-independent-set: all assertions passed\n");
    return 0;
}
