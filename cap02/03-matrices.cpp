// cap02/03-matrices.cpp
// 2D matrices using vector<vector<int>>
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o matrices 03-matrices.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fematrices.exe 03-matrices.cpp

#include <algorithm>
#include <iostream>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

using Vi  = std::vector<int>;
using Vvi = std::vector<std::vector<int>>;

// ---- Construction ----

// Zero-initialized rows by cols matrix, O(rows*cols)
Vvi make_matrix(int rows, int cols, int init = 0) {
    return Vvi(rows, Vi(cols, init));
}

// Read from stdin: first line = n m, then n lines of m integers each.
// O(n*m)
Vvi read_matrix(int rows, int cols) {
    Vvi mat = make_matrix(rows, cols);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            std::cin >> mat[r][c];
    return mat;
}

// ---- Display ----

void print_matrix(const Vvi& mat) {                     // O(n*m)
    for (const auto& row : mat) {
        for (auto x : row) std::print("{:4}", x);
        std::print("\n");
    }
}

// ---- Row/column operations ----

// Sum of row r: O(m)
long long row_sum(const Vvi& mat, int r) {
    return std::accumulate(mat[r].begin(), mat[r].end(), 0LL);
}

// Maximum in column c: O(n)
int col_max(const Vvi& mat, int c) {
    int best = mat[0][c];
    for (const auto& row : mat) best = std::max(best, row[c]);
    return best;
}

// In-place transpose (square matrix only): O(n^2)
void transpose(Vvi& mat) {
    int n = static_cast<int>(mat.size());
    for (int r = 0; r < n; ++r)
        for (int c = r + 1; c < n; ++c)
            std::swap(mat[r][c], mat[c][r]);
}

// Sort each row independently: O(n * m log m)
void sort_rows(Vvi& mat) {
    for (auto& row : mat) std::ranges::sort(row);
}

// Sort rows by their sum (ascending): O(n * m + n log n)
void sort_rows_by_sum(Vvi& mat) {
    std::ranges::sort(mat, [](const Vi& a, const Vi& b) {
        auto sa = std::accumulate(a.begin(), a.end(), 0LL);
        auto sb = std::accumulate(b.begin(), b.end(), 0LL);
        return sa < sb;
    });
}

// ---- Prefix sum matrix ----
// prefix[r][c] = sum of all elements in the rectangle (0,0)-(r-1,c-1).
// Build: O(n*m). Query rectangle [r1,c1]-[r2,c2]: O(1).
Vvi build_prefix(const Vvi& mat) {
    int n = static_cast<int>(mat.size());
    int m = static_cast<int>(mat[0].size());
    Vvi prefix = make_matrix(n + 1, m + 1);
    for (int r = 1; r <= n; ++r)
        for (int c = 1; c <= m; ++c)
            prefix[r][c] = mat[r-1][c-1]
                         + prefix[r-1][c]
                         + prefix[r][c-1]
                         - prefix[r-1][c-1];
    return prefix;
}

// Query sum in rectangle [r1,c1]-[r2,c2] (inclusive, 0-indexed): O(1)
int query(const Vvi& prefix, int r1, int c1, int r2, int c2) {
    return prefix[r2+1][c2+1]
         - prefix[r1][c2+1]
         - prefix[r2+1][c1]
         + prefix[r1][c1];
}

int main() {
    Vvi mat = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::print("Original:\n");
    print_matrix(mat);

    std::print("row_sum(1) = {}\n", row_sum(mat, 1));   // 15
    std::print("col_max(2) = {}\n", col_max(mat, 2));   // 9

    transpose(mat);
    std::print("Transposed:\n");
    print_matrix(mat);

    // Prefix sum demo
    Vvi m2 = {{1,2,3},{4,5,6},{7,8,9}};
    auto prefix = build_prefix(m2);
    // Sum of submatrix (0,0)-(1,1): 1+2+4+5 = 12
    std::print("prefix query (0,0)-(1,1) = {}\n", query(prefix, 0, 0, 1, 1)); // 12
    // Sum of entire matrix
    std::print("prefix query (0,0)-(2,2) = {}\n", query(prefix, 0, 0, 2, 2)); // 45
}
