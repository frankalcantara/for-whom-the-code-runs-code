// Exercise T09.3 - The Banquet Prep
// Session: T09.S3, exercise 1 of 2 (1 hour for both)
// Theme: chapter 09 (topological sort, Kahn's algorithm, cycle
//   detection by counting)
// Tags: chapter-09, sorting, topological-sort
//
// MODEL: the rules are a directed graph on tasks; a valid work order
//   is a topological order, which exists iff the graph is a DAG. A
//   contradiction in the rules is precisely a directed cycle.
// MATH: Kahn's algorithm: compute in-degrees; start with all
//   zero-in-degree tasks; repeatedly emit one and decrement its
//   successors, releasing those that reach zero. If the emitted count
//   is n, the emission order is topological; if less, the leftover
//   subgraph has minimum in-degree >= 1 and therefore contains a
//   cycle (follow any predecessor chain; by pigeonhole it repeats).
// PROOF: every emitted task has all predecessors emitted earlier
//   (it reached in-degree zero only after each rule a -> b had its a
//   emitted), so the order satisfies every rule. Completeness: if a
//   topological order exists, its first unemitted task always has
//   in-degree zero among unemitted tasks, so the queue never dries up
//   early; contrapositive gives the cycle certificate above.
// COMPLEXITY: O(n + m) time and memory - each task enters the queue
//   once, each rule is decremented once. Sorting-based approaches do
//   not apply: precedence is a partial order, not a comparison.
// TYPES: in-degrees and ids fit int. The output line is up to 2e5
//   numbers - assembled in one string and written once (Chapter 3's
//   buffered-output habit, now reflexive).
// ALTERNATIVES: DFS finish-time order (reversed) is the other
//   standard topological sort; it finds cycles via the gray-vertex
//   test but recurses n deep - iterative DFS or Kahn is the safer
//   contest posture. A priority queue instead of a plain queue gives
//   the lexicographically smallest order (not requested; the cook
//   accepts any obedient apprentice).
// EDGE CASES: m = 0 (any order works); duplicate rules (in-degree
//   counted twice, decremented twice - still correct); a -> a
//   self-rule (immediate contradiction); two disjoint chains
//   (interleaving allowed); the full-cycle input (emits nothing).
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o prep t09-s3-1-banquet-prep.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feprep.exe t09-s3-1-banquet-prep.cpp

#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<int>> next(static_cast<std::size_t>(n) + 1);
    std::vector<int> indegree(static_cast<std::size_t>(n) + 1, 0);
    for (int e = 0; e < m; ++e) {
        int a = 0;
        int b = 0;
        std::cin >> a >> b;
        next[static_cast<std::size_t>(a)].push_back(b);
        ++indegree[static_cast<std::size_t>(b)];
    }

    std::queue<int> ready;
    for (int t = 1; t <= n; ++t) {
        if (indegree[static_cast<std::size_t>(t)] == 0) ready.push(t);
    }

    std::string out;
    out.reserve(static_cast<std::size_t>(n) * 7);
    int emitted = 0;
    while (!ready.empty()) {
        const int t = ready.front();
        ready.pop();
        ++emitted;
        if (!out.empty()) out.push_back(' ');
        out += std::to_string(t);
        for (const int s : next[static_cast<std::size_t>(t)]) {
            if (--indegree[static_cast<std::size_t>(s)] == 0) ready.push(s);
        }
    }

    if (emitted < n) {
        std::fputs("contradiction\n", stdout);
    } else {
        out.push_back('\n');
        std::fwrite(out.data(), 1, out.size(), stdout);
    }
    return 0;
}
