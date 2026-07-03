// cap09/16-csr-vs-adjacency.cpp
// Two ways to store a sparse graph and the cost of traversing it. The textbook
// std::vector<std::vector<int>> is V separate allocations chased by pointer;
// compressed sparse row (CSR) packs every neighbor list end to end in one
// array, with an offset table that says where each vertex's list begins. CSR
// builds with a counting pass (degrees, prefix sums) and traverses contiguous
// memory. The benchmark sweeps every edge of both and times it.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o csr 16-csr-vs-adjacency.cpp

#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

template <class F>
static double time_ms(F&& f) {
    auto t0 = Clock::now();
    volatile long long sink = f();
    (void)sink;
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    const int V = 2'000'000;
    const int E = 8'000'000;

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> vtx(0, V - 1);
    std::vector<int> us(E), vs(E);
    for (int e = 0; e < E; ++e) { us[e] = vtx(rng); vs[e] = vtx(rng); }

    // (a) adjacency list: one vector per vertex.
    std::vector<std::vector<int>> adj(V);
    for (int e = 0; e < E; ++e) adj[us[e]].push_back(vs[e]);

    // (b) CSR: head[u]..head[u+1] is u's slice of the flat `to` array.
    std::vector<int> head(V + 1, 0);
    for (int e = 0; e < E; ++e) ++head[us[e] + 1];
    for (int i = 0; i < V; ++i) head[i + 1] += head[i];   // prefix sums = offsets
    std::vector<int> to(E);
    {
        std::vector<int> cur(head.begin(), head.end());
        for (int e = 0; e < E; ++e) to[cur[us[e]]++] = vs[e];
    }

    double t_adj = time_ms([&] {
        long long acc = 0;
        for (int u = 0; u < V; ++u)
            for (int w : adj[u]) acc += w;
        return acc;
    });
    double t_csr = time_ms([&] {
        long long acc = 0;
        for (int u = 0; u < V; ++u)
            for (int i = head[u]; i < head[u + 1]; ++i) acc += to[i];
        return acc;
    });

    std::println("adjacency list traversal: {:.1f} ms", t_adj);
    std::println("CSR traversal           : {:.1f} ms", t_csr);
    return 0;
}
