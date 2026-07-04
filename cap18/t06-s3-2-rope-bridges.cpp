// Exercise T06.4 - The Rope Bridges
// Session: T06.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 06 (DSU; incremental connectivity, union by size + path
//   compression)
// Tags: chapter-06, dsu, number-theory
//
// MODEL: incremental connectivity: edges only arrive, never leave, and
//   queries ask "same component?". This is the DSU contract; the absence
//   of deletions is what makes the structure applicable at all.
// MATH: a forest of rooted trees, one per component. find(x) walks to
//   the root; union links the root of the smaller tree under the root of
//   the larger (union by size); path compression points every visited
//   node directly at the root during find.
// PROOF: correctness is the invariant "two huts have the same root iff
//   a sequence of built bridges connects them", preserved by both
//   operations (union merges exactly the two components of its
//   endpoints; compression rewires within one tree, never across). The
//   cost bound: union by size alone gives trees of height O(log n)
//   (each link at least doubles the size of the subtree a node belongs
//   to); with compression the amortized cost per operation drops to the
//   inverse Ackermann alpha(n), constant for every n that fits in this
//   universe. The chapter states the theorem; the proof lives in the
//   references and does not need to be re-derived to be used honestly.
// COMPLEXITY: O((n + q) * alpha(n)) time, O(n) memory. BFS per question
//   is O(q * (n + q)) = 8e10; rebuilding components per bridge is worse.
// TYPES: everything is an index or a size bounded by n = 2e5: int
//   throughout, and the parent array doubles as the size array under the
//   classic negative-value encoding (parent[root] = -size).
// ALTERNATIVES: union by rank (interchangeable); link-cut trees support
//   deletions this statement was careful not to ask for; offline
//   processing cannot help because answers must come "even while
//   construction continues".
// EDGE CASES: question before any bridge (NO unless a = b); a = b
//   (always YES, a hut reaches itself); duplicate bridges (union of the
//   same component is a no-op); bridge joining already-connected huts
//   via a longer path; n = 1.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o bridges t06-s3-2-rope-bridges.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Febridges.exe t06-s3-2-rope-bridges.cpp

#include <iostream>
#include <print>
#include <vector>

class Dsu {
public:
    explicit Dsu(int n) : parent_(n + 1, -1) {}

    int find(int x) {
        int root = x;
        while (parent_[root] >= 0) root = parent_[root];
        while (parent_[x] >= 0) {  // path compression
            int next = parent_[x];
            parent_[x] = root;
            x = next;
        }
        return root;
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (parent_[a] > parent_[b]) std::swap(a, b);  // a is the bigger tree
        parent_[a] += parent_[b];
        parent_[b] = a;
    }

private:
    std::vector<int> parent_;  // parent, or -size at the root
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    Dsu dsu(n);
    while (q-- > 0) {
        int type = 0;
        int a = 0;
        int b = 0;
        std::cin >> type >> a >> b;
        if (type == 1) {
            dsu.unite(a, b);
        } else {
            std::print("{}\n", dsu.find(a) == dsu.find(b) ? "YES" : "NO");
        }
    }
    return 0;
}
