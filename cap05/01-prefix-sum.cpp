// cap05/01-prefix-sum.cpp
// Prefix sum in 1D and 2D -- construction and range-sum queries.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o prefix_sum 01-prefix-sum.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feprefix_sum.exe 01-prefix-sum.cpp

#include <cassert>
#include <numeric>
#include <print>
#include <vector>

using Vi  = std::vector<int>;
using Vvi = std::vector<std::vector<int>>;
using i64 = long long;

// ============================================================
// Prefix sum 1D
// Build: O(n)   Query sum[l, r]: O(1)
// Convention: closed interval [l, r]
// ============================================================
struct PrefixSum1D {
    std::vector<i64> p;  // p[i] = a[0] + ... + a[i-1], p[0] = 0

    explicit PrefixSum1D(const Vi& a) : p(a.size() + 1, 0) {
        for (int i = 0; i < static_cast<int>(a.size()); ++i)
            p[i + 1] = p[i] + a[i];
    }

    // Sum of the closed interval [l, r]: O(1)
    i64 query(int l, int r) const { return p[r + 1] - p[l]; }
};

// ============================================================
// Prefix sum 2D
// Build: O(nm)   Query sum of rectangle [r1,c1]-[r2,c2]: O(1)
// Convention: 0-indexed, closed rectangle [r1,c1]-[r2,c2]
// ============================================================
struct PrefixSum2D {
    int rows, cols;                           // declared before p
    std::vector<std::vector<i64>> p;

    explicit PrefixSum2D(const Vvi& mat)
        : rows(static_cast<int>(mat.size()))
        , cols(static_cast<int>(mat[0].size()))
        , p(rows + 1, std::vector<i64>(cols + 1, 0))
    {
        // 2D inclusion-exclusion: p[r][c] is the sum from (0,0) to (r-1,c-1).
        for (int r = 1; r <= rows; ++r)
            for (int c = 1; c <= cols; ++c)
                p[r][c] = mat[r-1][c-1]
                         + p[r-1][c] + p[r][c-1]
                         - p[r-1][c-1];
    }

    // Sum of rectangle [r1,c1]-[r2,c2] (0-indexed, closed): O(1)
    i64 query(int r1, int c1, int r2, int c2) const {
        return p[r2+1][c2+1] - p[r1][c2+1] - p[r2+1][c1] + p[r1][c1];
    }
};

int main() {
    // --- 1D ---
    Vi a = {3, 1, 4, 1, 5, 9, 2, 6};
    PrefixSum1D ps(a);

    assert(ps.query(0, 7) == 31);          // total sum
    assert(ps.query(2, 4) == 4 + 1 + 5);  // a[2..4]
    assert(ps.query(0, 0) == 3);           // only a[0]

    i64 v1d = ps.query(2, 4);
    std::print("prefix 1D: sum[2,4] = {}\n", v1d);

    // --- 2D ---
    Vvi mat = {{1,2,3},{4,5,6},{7,8,9}};
    PrefixSum2D ps2(mat);

    assert(ps2.query(0, 0, 2, 2) == 45);        // toda a matriz 3x3
    assert(ps2.query(1, 1, 2, 2) == 5+6+8+9);   // submatriz 2x2 inferior direita
    assert(ps2.query(0, 0, 0, 0) == 1);

    i64 v2d = ps2.query(1, 1, 2, 2);
    std::print("prefix 2D: submatrix [1,1]-[2,2] = {}\n", v2d);
    std::print("01-prefix-sum: all assertions passed\n");
    return 0;
}
