// cap07/03-sudoku.cpp
// Sudoku solver with backtracking + constraint propagation.
// Technique: for each empty cell, try digits 1-9;
// constraint propagation removes impossible candidates with bitmasks.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o sudoku 03-sudoku.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesudoku.exe 03-sudoku.cpp

#include <cassert>
#include <array>
#include <bit>
#include <print>

// ============================================================
// Representation: grid[9][9], 0 = empty.
// Candidate bitmasks: rows[r], cols[c], boxes[b] -- bit d means digit d is used.
// ============================================================
struct Sudoku {
    std::array<std::array<int,9>,9> g{};
    std::array<int,9> rows{}, cols{}, boxes{};

    // 3x3 box id for (r,c).
    static int box(int r, int c) { return (r/3)*3 + c/3; }

    // Valid candidates for (r,c): digits not used in row/column/box.
    int candidates(int r, int c) const {
        int used = rows[r] | cols[c] | boxes[box(r,c)];
        return (~used) & 0x1FF;  // 9 bits: bit i-1 = digit i is available
    }

    void place(int r, int c, int d) {
        g[r][c] = d;
        int bit = 1 << (d - 1);
        rows[r] |= bit; cols[c] |= bit; boxes[box(r,c)] |= bit;
    }

    void remove(int r, int c, int d) {
        g[r][c] = 0;
        int bit = 1 << (d - 1);
        rows[r] &= ~bit; cols[c] &= ~bit; boxes[box(r,c)] &= ~bit;
    }

    // Initializes from an 81-character string ('0' = empty).
    void load(const char* s) {
        for (int i = 0; i < 81; ++i) {
            int r = i/9, c = i%9;
            int d = s[i] - '0';
            if (d) place(r, c, d);
        }
    }

    // Backtracking with bitmask constraint propagation.
    bool solve() {
        // Chooses the empty cell with the fewest candidates (MRV heuristic).
        int best_r = -1, best_c = -1, best_cnt = 10;
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (g[r][c] != 0) continue;
                int cands = candidates(r, c);
                int cnt = std::popcount(static_cast<unsigned>(cands));
                if (cnt == 0) return false;  // no candidates -- backtrack
                if (cnt < best_cnt) {
                    best_cnt = cnt; best_r = r; best_c = c;
                }
            }
        }
        if (best_r == -1) return true;  // all cells filled

        int cands = candidates(best_r, best_c);
        while (cands) {
            int bit = cands & (-cands);
            cands &= cands - 1;
            int d = std::countr_zero(static_cast<unsigned>(bit)) + 1;

            place(best_r, best_c, d);
            if (solve()) return true;
            remove(best_r, best_c, d);
        }
        return false;
    }

    void print_grid() const {
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c)
                std::print("{}", g[r][c]);
            std::print("\n");
        }
    }
};

int main() {
    // "Hard" puzzle from Project Euler problem 96.
    const char* puzzle =
        "003020600"
        "900305001"
        "001806400"
        "008102900"
        "700000008"
        "006708200"
        "002609500"
        "800203009"
        "005010300";

    const char* solution =
        "483921657"
        "967345821"
        "251876493"
        "548132976"
        "729564138"
        "136798245"
        "372689514"
        "814253769"
        "695417382";

    Sudoku s;
    s.load(puzzle);
    bool ok = s.solve();
    assert(ok);

    // Verifies solution.
    for (int i = 0; i < 81; ++i) {
        int r = i/9, c = i%9;
        assert(s.g[r][c] == solution[i] - '0');
    }

    s.print_grid();
    std::print("03-sudoku: all assertions passed\n");
    return 0;
}
