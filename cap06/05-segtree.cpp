// cap06/05-segtree.cpp
// Segment Tree with lazy propagation -- range update + range query.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o segtree 05-segtree.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesegtree.exe 05-segtree.cpp

#include <algorithm>
#include <cassert>
#include <climits>
#include <print>
#include <vector>

using i64 = long long;

// ============================================================
// Generic segment tree with lazy propagation.
// Supports:
//   - range update: adds v to every element in [l,r]
//   - range query: sum or minimum of [l,r]
//
// Node: stores sum and min. Lazy: pending range addition.
// Propagation: before descending, pushes the lazy value to the children.
// ============================================================
struct SegTree {
    int n;
    std::vector<i64> sum, mn, lazy;

    explicit SegTree(int n) : n(n), sum(4*n, 0), mn(4*n, 0), lazy(4*n, 0) {}

    explicit SegTree(const std::vector<int>& a)
        : n(static_cast<int>(a.size()))
        , sum(4*n, 0), mn(4*n, 0), lazy(4*n, 0)
    {
        build(a, 1, 0, n - 1);
    }

    void push_up(int node) {
        sum[node] = sum[2*node] + sum[2*node+1];
        mn[node]  = std::min(mn[2*node], mn[2*node+1]);
    }

    // Applies lazy to node (l..r): every element receives +v.
    void apply(int node, int l, int r, i64 v) {
        sum[node]  += v * (r - l + 1);
        mn[node]   += v;
        lazy[node] += v;
    }

    void push_down(int node, int l, int r) {
        if (lazy[node] == 0) return;
        int mid = (l + r) / 2;
        apply(2*node,   l,      mid, lazy[node]);
        apply(2*node+1, mid+1,  r,   lazy[node]);
        lazy[node] = 0;
    }

    void build(const std::vector<int>& a, int node, int l, int r) {
        if (l == r) { sum[node] = mn[node] = a[l]; return; }
        int mid = (l + r) / 2;
        build(a, 2*node,   l,      mid);
        build(a, 2*node+1, mid+1,  r);
        push_up(node);
    }

    // Adds v to every element in [ql, qr]: O(log n).
    void update(int node, int l, int r, int ql, int qr, i64 v) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) { apply(node, l, r, v); return; }
        push_down(node, l, r);
        int mid = (l + r) / 2;
        update(2*node,   l,     mid, ql, qr, v);
        update(2*node+1, mid+1, r,   ql, qr, v);
        push_up(node);
    }

    // Sum of [ql, qr]: O(log n).
    i64 query_sum(int node, int l, int r, int ql, int qr) const {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return sum[node];
        int mid = (l + r) / 2;
        // push_down mutates lazy state; this const query keeps the public API simple.
        const_cast<SegTree*>(this)->push_down(node, l, r);
        return query_sum(2*node,   l,     mid, ql, qr)
             + query_sum(2*node+1, mid+1, r,   ql, qr);
    }

    // Minimum of [ql, qr]: O(log n).
    i64 query_min(int node, int l, int r, int ql, int qr) const {
        if (ql > r || qr < l) return LLONG_MAX;
        if (ql <= l && r <= qr) return mn[node];
        int mid = (l + r) / 2;
        const_cast<SegTree*>(this)->push_down(node, l, r);
        return std::min(query_min(2*node,   l,     mid, ql, qr),
                        query_min(2*node+1, mid+1, r,   ql, qr));
    }

    // Public interfaces hide the root-node parameters.
    void   update(int l, int r, i64 v) { update(1, 0, n-1, l, r, v); }
    i64    query_sum(int l, int r)  const { return query_sum(1, 0, n-1, l, r); }
    i64    query_min(int l, int r)  const { return query_min(1, 0, n-1, l, r); }
};

int main() {
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6};
    SegTree st(a);

    // Initial sum
    assert(st.query_sum(0, 7) == 31);
    assert(st.query_sum(2, 4) == 4 + 1 + 5);

    // Initial minimum
    assert(st.query_min(0, 7) == 1);
    assert(st.query_min(5, 7) == 2);

    std::print("sum[0,7]={}, min[5,7]={}\n", st.query_sum(0,7), st.query_min(5,7));

    // Range update: a[2..5] += 10
    st.update(2, 5, 10);
    assert(st.query_sum(2, 4) == 14 + 11 + 15);  // 4+10, 1+10, 5+10
    assert(st.query_min(2, 5) == 11);              // min(14,11,15,19) = 11

    std::print("after update[2,5]+=10: sum[2,4]={}, min[2,5]={}\n",
        st.query_sum(2, 4), st.query_min(2, 5));

    // Nested range update
    st.update(0, 7, -5);  // subtracts 5 from all elements
    assert(st.query_min(2, 5) == 6);  // 11 - 5

    std::print("05-segtree: all assertions passed\n");
    return 0;
}
