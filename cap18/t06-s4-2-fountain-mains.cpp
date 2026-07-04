// Exercise T06.6 - The Fountain Mains
// Session: T06.S4, exercise 2 of 3 (2 hours for the three)
// Theme: chapter 06 (Kruskal; sort by weight + DSU acceptance test)
// Tags: chapter-06, sorting, greedy, dsu
//
// MODEL: minimum spanning tree of a weighted undirected graph, or the
//   verdict that the graph is disconnected.
// MATH: sort the m mains by cost; sweep in order, accepting a main iff
//   its endpoints lie in different DSU components; stop logically after
//   n - 1 acceptances. The accepted set is a spanning tree of minimum
//   total weight.
// PROOF: the cut property. When Kruskal examines the cheapest edge e
//   crossing the cut between the component of a and the rest, no cheaper
//   crossing edge exists (all cheaper edges were already processed and
//   rejected only when they crossed nothing); exchanging e into any
//   spanning tree that omits it removes a strictly-no-cheaper edge of
//   the cycle e closes, so some MST contains e: greedy choice is safe,
//   induction over acceptances finishes. Fewer than n - 1 acceptances at
//   the end certify disconnection, because a connected graph always
//   offers a crossing edge to a sweep that never skips one wrongly.
// COMPLEXITY: O(m log m) for the sort, O(m * alpha(n)) for the sweep:
//   the sort dominates. Memory O(n + m).
// TYPES: total cost reaches (n - 1) * 1e9 ~ 2e14: long long for the
//   accumulator; weights themselves fit in int. Indices int.
// ALTERNATIVES: Prim with a heap, O(m log n), better on dense graphs
//   (m ~ n^2) where Kruskal's sort hurts; Boruvka, the parallel-friendly
//   one. At m = 2e5 the sort costs nothing and Kruskal is the least
//   code. Checking connectivity first with a BFS would scan the graph a
//   second time to learn what the DSU already knows.
// EDGE CASES: n = 1 (cost 0, no main needed); m = 0 with n > 1
//   (impossible); parallel mains between the same pair (the cheaper one
//   is seen first, the second is rejected); self-loops a = b (rejected
//   by the same test); already-spanning prefix (later mains all
//   rejected); the 2e14 total.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o fountains t06-s4-2-fountain-mains.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fefountains.exe t06-s4-2-fountain-mains.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

struct Main {
    int a, b, w;
};

class Dsu {
public:
    explicit Dsu(int n) : parent_(n + 1, -1) {}

    int find(int x) {
        int root = x;
        while (parent_[root] >= 0) root = parent_[root];
        while (parent_[x] >= 0) {
            int next = parent_[x];
            parent_[x] = root;
            x = next;
        }
        return root;
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (parent_[a] > parent_[b]) std::swap(a, b);
        parent_[a] += parent_[b];
        parent_[b] = a;
        return true;
    }

private:
    std::vector<int> parent_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<Main> mains(m);
    for (auto& e : mains) std::cin >> e.a >> e.b >> e.w;

    std::ranges::sort(mains, {}, &Main::w);

    Dsu dsu(n);
    long long total = 0;
    int accepted = 0;
    for (const auto& e : mains) {
        if (dsu.unite(e.a, e.b)) {
            total += e.w;
            if (++accepted == n - 1) break;
        }
    }

    if (accepted == n - 1) {
        std::println("{}", total);
    } else {
        std::println("impossible");
    }
    return 0;
}
