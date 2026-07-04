// cap05/04-sparse-table.cpp
// Sparse Table for static RMQ (Range Minimum Query).
// Build: O(n log n)   Query: O(1) -- overlap is valid for min/max (idempotent).
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o sparse_table 04-sparse-table.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesparse_table.exe 04-sparse-table.cpp

#include <bit>        // std::bit_width (C++20) for floor(log2)
#include <cassert>
#include <climits>
#include <print>
#include <vector>

using Vi  = std::vector<int>;
using Vvi = std::vector<std::vector<int>>;

// ============================================================
// Sparse Table -- static RMQ.
// st[k][i] = minimum of a[i .. i + 2^k - 1]
// Two intervals of size 2^k overlap to cover [l, r]:
//   min(st[k][l], st[k][r - 2^k + 1])  onde k = floor(log2(r-l+1))
// The overlap is safe because min is idempotent: min(x,x) = x.
// ============================================================
struct SparseTable {
    Vvi st;
    Vi  log2_;   // log2_[i] = floor(log2(i))
    int n;

    explicit SparseTable(const Vi& a)
        : n(static_cast<int>(a.size()))
        , log2_(n + 1, 0)
    {
        // Precomputes floor(log2) for all lengths up to n -- O(n).
        for (int i = 2; i <= n; ++i)
            log2_[i] = log2_[i / 2] + 1;

        int levels = log2_[n] + 1;
        st.assign(levels, Vi(n, INT_MAX));
        st[0] = a;  // level 0: intervals of length 1

        // st[k][i] = min(st[k-1][i], st[k-1][i + 2^(k-1)])
        for (int k = 1; k < levels; ++k)
            for (int i = 0; i + (1 << k) <= n; ++i)
                st[k][i] = std::min(st[k-1][i], st[k-1][i + (1 << (k-1))]);
        // Total: O(n log n) time and space.
    }

    // Minimum on [l, r] (closed). O(1).
    int query(int l, int r) const {
        int k = log2_[r - l + 1];           // largest power of two that fits
        return std::min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

int main() {
    Vi a = {3, 2, 1, 5, 4, 6, 7, 2};
    SparseTable rmq(a);

    assert(rmq.query(0, 7) == 1);   // global minimum
    assert(rmq.query(0, 2) == 1);   // {3,2,1}
    assert(rmq.query(3, 5) == 4);   // {5,4,6}
    assert(rmq.query(5, 7) == 2);   // {6,7,2}
    assert(rmq.query(1, 1) == 2);   // single element

    std::print("rmq[0,7]={} rmq[3,5]={} rmq[5,7]={}\n",
               rmq.query(0,7), rmq.query(3,5), rmq.query(5,7));
    std::print("04-sparse-table: all assertions passed\n");
    return 0;
}
