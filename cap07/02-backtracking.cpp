// cap07/02-backtracking.cpp
// Backtracking: N-Queens and permutation/combination generation.
//
// N-Queens naive: O(n!) -- tries column placements with conflict checks.
// Optimized N-Queens: bitmasks for columns and diagonals with efficient pruning.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o backtracking 02-backtracking.cpp

#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <print>
#include <vector>

// ============================================================
// Layer 1 -- Naive N-Queens: column array with explicit conflict checks.
// Places one queen per row; checks columns and diagonals.
// ============================================================
static int naive_count = 0;

void naive_nqueens(int row, int n, std::vector<int>& cols) {
    if (row == n) { ++naive_count; return; }
    for (int c = 0; c < n; ++c) {
        bool ok = true;
        for (int r = 0; r < row; ++r) {
            if (cols[r] == c || std::abs(cols[r] - c) == row - r) {
                ok = false; break;
            }
        }
        if (ok) {
            cols[row] = c;
            naive_nqueens(row + 1, n, cols);
        }
    }
}

int nqueens_naive(int n) {
    naive_count = 0;
    std::vector<int> cols(n, -1);
    naive_nqueens(0, n, cols);
    return naive_count;
}

// ============================================================
// Layer 2 -- N-Queens with bitmasks: same search class, smaller constant.
// Three bitmasks: cols (occupied columns), diag1, diag2.
//   diag1[r][c] = r+c is constant on a main diagonal.
//   diag2[r][c] = r-c is constant on a secondary diagonal.
// Bitmasks find free positions with bit operations.
// ============================================================
static int bitmask_count = 0;

// full = (1 << n) - 1: all columns occupied means a solution was found.
void bitmask_nqueens(int cols, int diag1, int diag2, int full) {
    if (cols == full) { ++bitmask_count; return; }

    // Available positions: 1 = free.
    int avail = full & ~(cols | diag1 | diag2);

    // Iterates through set bits by extracting the least significant bit.
    while (avail) {
        int bit = avail & (-avail);  // lowbit: rightmost available position
        avail &= avail - 1;          // removes that bit

        bitmask_nqueens(
            cols  | bit,
            (diag1 | bit) << 1,   // main diagonal shifts left
            (diag2 | bit) >> 1,   // secondary diagonal shifts right
            full
        );
    }
}

int nqueens_bitmask(int n) {
    bitmask_count = 0;
    bitmask_nqueens(0, 0, 0, (1 << n) - 1);
    return bitmask_count;
}

// ============================================================
// Permutation generation with backtracking -- O(n! * n) if all are stored.
// ============================================================
void gen_permutations(std::vector<int>& a, int pos,
                       std::vector<std::vector<int>>& result) {
    if (pos == static_cast<int>(a.size())) {
        result.push_back(a);
        return;
    }
    for (int i = pos; i < static_cast<int>(a.size()); ++i) {
        std::swap(a[pos], a[i]);
        gen_permutations(a, pos + 1, result);
        std::swap(a[pos], a[i]);  // backtrack
    }
}

// ============================================================
// Combination generation C(n, k) with backtracking.
// ============================================================
void gen_combinations(int n, int k, int start, std::vector<int>& cur,
                       std::vector<std::vector<int>>& result) {
    if (static_cast<int>(cur.size()) == k) {
        result.push_back(cur);
        return;
    }
    // Pruning: are there enough remaining values?
    int remaining = n - start + 1;
    int needed    = k - static_cast<int>(cur.size());
    if (remaining < needed) return;

    for (int i = start; i <= n; ++i) {
        cur.push_back(i);
        gen_combinations(n, k, i + 1, cur, result);
        cur.pop_back();  // backtrack
    }
}

int main() {
    // N-Queens: known results
    // n:   1  2  3  4   5    6    7     8
    // sol: 1  0  0  2  10   4   40    92
    int expected[] = {1, 0, 0, 2, 10, 4, 40, 92};
    for (int n = 1; n <= 8; ++n) {
        int naive   = nqueens_naive(n);
        int bitmask = nqueens_bitmask(n);
        assert(naive == expected[n-1]);
        assert(bitmask == expected[n-1]);
    }
    std::print("N-Queens(8) = {} solutions\n", nqueens_bitmask(8));

    // Permutations of {1,2,3}: 3! = 6
    std::vector<int> a = {1, 2, 3};
    std::vector<std::vector<int>> perms;
    gen_permutations(a, 0, perms);
    assert(static_cast<int>(perms.size()) == 6);
    std::print("permutations of {{1,2,3}}: {}\n", perms.size());

    // Combinations C(5,2) = 10
    std::vector<std::vector<int>> combs;
    std::vector<int> cur;
    gen_combinations(5, 2, 1, cur, combs);
    assert(static_cast<int>(combs.size()) == 10);
    std::print("C(5,2) = {}\n", combs.size());

    std::print("02-backtracking: all assertions passed\n");
    return 0;
}
