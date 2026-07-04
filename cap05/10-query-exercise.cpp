// cap05/10-query-exercise.cpp
// Solved exercise: Range Sum and Range Minimum Queries
//
// Given n integers and q queries, each query asks for:
//   - the sum of a[l..r]
//   - the minimum of a[l..r]
//
// Layer 1 (naive): O(nq) -- scans the subarray for each query.
// Layer 2 (algorithmic): Prefix Sum (sum O(1)) + Sparse Table (min O(1))
//                        after O(n log n) preprocessing.
//
// Input:  line 1 = n q
//         line 2 = n integers
//         lines 3..q+2 = l r
// Output: for each query: sum min
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o queries 10-query-exercise.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fequeries.exe 10-query-exercise.cpp
// Test:    printf "8 4\n3 2 1 5 4 6 7 2\n0 7\n2 5\n3 3\n1 6\n" | ./queries

#include <cassert>
#include <climits>
#include <iostream>
#include <numeric>
#include <print>
#include <vector>

using Vi  = std::vector<int>;
using i64 = long long;

// ============================================================
// Layer 1 -- Naive: O(n) per query.
// ============================================================
std::pair<i64,int> naive_query(const Vi& a, int l, int r) {
    i64 sum = 0; int mn = INT_MAX;
    for (int i = l; i <= r; ++i) { sum += a[i]; mn = std::min(mn, a[i]); }
    return {sum, mn};
}

// ============================================================
// Layer 2 -- Prefix Sum + Sparse Table: O(1) per query.
// ============================================================
struct PrefixSum {
    std::vector<i64> p;
    explicit PrefixSum(const Vi& a) : p(a.size() + 1, 0) {
        for (int i = 0; i < static_cast<int>(a.size()); ++i)
            p[i + 1] = p[i] + a[i];
    }
    i64 query(int l, int r) const { return p[r + 1] - p[l]; }
};

struct SparseTable {
    std::vector<std::vector<int>> st;
    Vi log2_;

    explicit SparseTable(const Vi& a) {
        int n = static_cast<int>(a.size());
        log2_.resize(n + 1, 0);
        for (int i = 2; i <= n; ++i) log2_[i] = log2_[i/2] + 1;

        int levels = log2_[n] + 1;
        st.assign(levels, Vi(n, INT_MAX));
        st[0] = a;
        for (int k = 1; k < levels; ++k)
            for (int i = 0; i + (1 << k) <= n; ++i)
                st[k][i] = std::min(st[k-1][i], st[k-1][i + (1 << (k-1))]);
    }

    int query(int l, int r) const {
        int k = log2_[r - l + 1];
        return std::min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    Vi a(n);
    for (int& x : a) std::cin >> x;

    PrefixSum  ps(a);
    SparseTable rmq(a);

    // Correctness verification: layer 2 == layer 1 for every query.
    bool verify = true;

    while (q--) {
        int l, r;
        std::cin >> l >> r;

        i64 sum = ps.query(l, r);
        int mn  = rmq.query(l, r);

        if (verify) {
            auto [ref_sum, ref_mn] = naive_query(a, l, r);
            assert(sum == ref_sum && mn == ref_mn);
        }

        std::print("{} {}\n", sum, mn);
    }
    return 0;
}
