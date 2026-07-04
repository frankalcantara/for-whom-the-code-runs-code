// Exercise T06.7 - The Rampart Torches
// Session: T06.S4, exercise 3 of 3 (2 hours for the three; the hard one)
// Theme: chapters 05-06 (segment tree with lazy propagation; range add,
//   range min — the update pattern a difference array handles only
//   offline, now forced online by interleaved queries)
// Tags: chapter-05, chapter-06, difference-array, segment-tree
//
// MODEL: an array under range-add and range-min, fully online. The
//   difference array from chapter 05 dies here: it answers nothing until
//   a full reconstruction, and the warden asks between updates.
// MATH: a segment tree over [1, n]; each node holds the minimum of its
//   interval plus a pending "lazy" addend not yet pushed to its
//   children. Range-add on [l, r] descends only into nodes whose
//   interval partially overlaps; fully covered nodes absorb d into both
//   min and lazy, in O(1), without visiting descendants. Range-min
//   descends the same way, pushing pending addends one level down before
//   trusting any child.
// PROOF: the invariant is "node.min + (sum of lazy values on the path
//   above it) equals the true minimum of its interval". Absorbing d at a
//   covered node preserves it (min commutes with +d: min(x_i + d) =
//   min(x_i) + d, the distributive law that makes add a valid lazy tag
//   over min); push() moves a tag one level down, which only reshuffles
//   the same path sums. Each update or query touches O(log n) nodes per
//   side: the standard two-frontier argument — at every depth, at most
//   two visited nodes are partially covered, and full covers terminate.
// COMPLEXITY: O(log n) per event, O((n + q) log n) ~ 2e5 * 18 * 2
//   operations in total; memory 4n nodes and 4n tags. The per-query
//   rebuild of a difference array is O(nq) = 4e10.
// TYPES: a reserve starts at up to 1e9 and can drift by q * 1e9 = 2e14
//   in either direction: long long for mins and tags. The recursive
//   depth is log2(2e5) ~ 18, no stack concern.
// ALTERNATIVES: Fenwick trees support range-add/range-sum (two trees)
//   but not range-min, whose inverse-free algebra Fenwick cannot
//   subtract; sqrt decomposition with per-block offsets and minima is
//   O(sqrt n) per op, a respectable fallback; offline reordering is
//   forbidden by the statement. Lazy segment tree is the canonical tool.
// EDGE CASES: l = r updates and queries; whole-wall operations (root
//   absorbs in O(1)); negative drift below zero (mins are negative);
//   consecutive updates with no query between (tags stack); query on a
//   region never updated; n = 1.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o rampart t06-s4-3-rampart-torches.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Ferampart.exe t06-s4-3-rampart-torches.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

class LazyMinTree {
public:
    explicit LazyMinTree(const std::vector<long long>& a)
        : n_(static_cast<int>(a.size())),
          min_(4 * a.size(), 0),
          lazy_(4 * a.size(), 0) {
        build(1, 0, n_ - 1, a);
    }

    void add(int l, int r, long long d) { add(1, 0, n_ - 1, l, r, d); }
    long long min(int l, int r) { return min(1, 0, n_ - 1, l, r); }

private:
    void build(int node, int lo, int hi, const std::vector<long long>& a) {
        if (lo == hi) {
            min_[node] = a[lo];
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * node, lo, mid, a);
        build(2 * node + 1, mid + 1, hi, a);
        min_[node] = std::min(min_[2 * node], min_[2 * node + 1]);
    }

    void push(int node) {
        if (lazy_[node] == 0) return;
        for (int child : {2 * node, 2 * node + 1}) {
            min_[child] += lazy_[node];
            lazy_[child] += lazy_[node];
        }
        lazy_[node] = 0;
    }

    void add(int node, int lo, int hi, int l, int r, long long d) {
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) {
            min_[node] += d;
            lazy_[node] += d;
            return;
        }
        push(node);
        int mid = (lo + hi) / 2;
        add(2 * node, lo, mid, l, r, d);
        add(2 * node + 1, mid + 1, hi, l, r, d);
        min_[node] = std::min(min_[2 * node], min_[2 * node + 1]);
    }

    long long min(int node, int lo, int hi, int l, int r) {
        if (l <= lo && hi <= r) return min_[node];
        push(node);
        int mid = (lo + hi) / 2;
        if (r <= mid) return min(2 * node, lo, mid, l, r);
        if (l > mid) return min(2 * node + 1, mid + 1, hi, l, r);
        return std::min(min(2 * node, lo, mid, l, r),
                        min(2 * node + 1, mid + 1, hi, l, r));
    }

    int n_;
    std::vector<long long> min_;
    std::vector<long long> lazy_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> oil(n);
    for (auto& x : oil) std::cin >> x;

    LazyMinTree tree(oil);
    while (q-- > 0) {
        int type = 0;
        int l = 0;
        int r = 0;
        std::cin >> type >> l >> r;
        if (type == 1) {
            long long d = 0;
            std::cin >> d;
            tree.add(l - 1, r - 1, d);
        } else {
            std::print("{}\n", tree.min(l - 1, r - 1));
        }
    }
    return 0;
}
