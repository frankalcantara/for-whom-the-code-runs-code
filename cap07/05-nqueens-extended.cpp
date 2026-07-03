// cap07/05-nqueens-extended.cpp
// Solved exercise: N-Queens with solution counting and first-solution capture.
//
// Part 1: count solutions -- bitmask backtracking.
// Part 2: generate the first solution as a grid.
// Part 3: count solutions for n=1..12 and verify against known values.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o nqueens_extended 05-nqueens-extended.cpp

#include <cassert>
#include <bit>
#include <cstdlib>
#include <print>
#include <vector>

// ============================================================
// Bitmask solver with first-solution capture.
// ============================================================
struct NQueens {
    int n, full;
    int count = 0;
    std::vector<int> first_solution;  // column[row] = c
    bool found_first = false;

    explicit NQueens(int n) : n(n), full((1 << n) - 1) {}

    void solve(int row, int cols, int diag1, int diag2,
               std::vector<int>& placement) {
        if (cols == full) {
            ++count;
            if (!found_first) {
                first_solution = placement;
                found_first = true;
            }
            return;
        }

        int avail = full & ~(cols | diag1 | diag2);
        while (avail) {
            int bit = avail & (-avail);
            avail &= avail - 1;
            int c = static_cast<int>(std::countr_zero(static_cast<unsigned>(bit)));

            placement[row] = c;
            solve(row + 1,
                  cols  | bit,
                  (diag1 | bit) << 1,
                  (diag2 | bit) >> 1,
                  placement);
        }
    }

    void run() {
        std::vector<int> placement(n, -1);
        solve(0, 0, 0, 0, placement);
    }

    void print_board() const {
        if (first_solution.empty()) { std::print("no solution\n"); return; }
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c)
                std::print("{}", first_solution[r] == c ? "Q" : ".");
            std::print("\n");
        }
    }
};

int main() {
    // Known values: OEIS A000170
    // n:  1  2  3  4   5   6    7     8      9     10      11       12
    int expected[] = {1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200};

    for (int n = 1; n <= 12; ++n) {
        NQueens solver(n);
        solver.run();
        assert(solver.count == expected[n-1]);
        std::print("N={}: {} solutions\n", n, solver.count);
    }

    // Generates and prints the first solution for n=8.
    NQueens solver8(8);
    solver8.run();
    std::print("\nFirst solution N=8:\n");
    solver8.print_board();

    // Verifies that the solution is valid: no conflicts.
    const auto& sol = solver8.first_solution;
    for (int r = 0; r < 8; ++r) {
        for (int r2 = r+1; r2 < 8; ++r2) {
            assert(sol[r] != sol[r2]);                     // distinct columns
            assert(std::abs(sol[r]-sol[r2]) != r2 - r);    // no diagonal conflict
        }
    }

    std::print("\n05-nqueens-extended: all assertions passed\n");
    return 0;
}
