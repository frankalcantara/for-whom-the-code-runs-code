// Exercise 1.5 - The Tiled Observatory
// Classic spelling: compare row-major and column-major traversal.
// Time: O(n^2). Space: O(n^2).

#include <chrono>
#include <iostream>
#include <limits>
#include <print>
#include <vector>

long long row_checksum(const std::vector<int>& matrix, int n) {
    // The matrix is stored as one row-major vector, so walking rows first follows
    // the same order used by the physical memory layout.
    long long sum = 0;

    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            // Consecutive columns consume consecutive elements, so each cache
            // line fetched from memory contributes several useful cells.
            // The cast moves the multiplication into the vector's index type.
            sum += matrix[static_cast<std::size_t>(row) * n + col];
        }
    }

    return sum;
}

long long column_checksum(const std::vector<int>& matrix, int n) {
    // This traversal deliberately keeps the same flat storage and changes only
    // the visiting order; that isolates memory locality as the variable tested.
    long long sum = 0;

    for (int col = 0; col < n; ++col) {
        for (int row = 0; row < n; ++row) {
            // The mathematical visit set is the same, but the stride through the
            // flat array is n elements instead of one, which wastes most values
            // brought in by each cache line for large n.
            sum += matrix[static_cast<std::size_t>(row) * n + col];
        }
    }

    return sum;
}

long long best_row_time(const std::vector<int>& matrix, int n,
                        long long& checksum) {
    // Start above every possible measurement so the first trial always becomes
    // the provisional best; later trials only replace it when they are faster.
    long long best = std::numeric_limits<long long>::max();

    // The caller receives a checksum because timing alone would not prove that
    // the traversal visited the intended cells.
    checksum = 0;

    for (int trial = 0; trial < 5; ++trial) {
        // Only the traversal is timed; matrix construction is outside this
        // function because the experiment is about access order, not setup cost.
        std::chrono::steady_clock::time_point begin =
            std::chrono::steady_clock::now();
        long long current = row_checksum(matrix, n);
        std::chrono::steady_clock::time_point end =
            std::chrono::steady_clock::now();

        long long elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                .count();
        if (elapsed < best) {
            best = elapsed;
        }

        // The checksum is kept observable so the compiler cannot treat the
        // traversal as a benchmark of unused work. XOR is cheap, and five
        // repeated identical checksums still leave the checksum value visible.
        checksum ^= current;
    }

    return best;
}

long long best_column_time(const std::vector<int>& matrix, int n,
                           long long& checksum) {
    // This mirrors best_row_time so both measurements differ only in traversal
    // order. Keeping the timing protocol identical makes the comparison fair.
    long long best = std::numeric_limits<long long>::max();
    checksum = 0;

    for (int trial = 0; trial < 5; ++trial) {
        // Repeating the same traversal several times reduces the influence of
        // one unlucky interruption by the operating system or another process.
        std::chrono::steady_clock::time_point begin =
            std::chrono::steady_clock::now();
        long long current = column_checksum(matrix, n);
        std::chrono::steady_clock::time_point end =
            std::chrono::steady_clock::now();

        long long elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                .count();
        if (elapsed < best) {
            best = elapsed;
        }

        // The row and column checksums must agree because both traversals visit
        // every cell exactly once; a mismatch would indicate an indexing bug.
        checksum ^= current;
    }

    return best;
}

int main() {
    int n = 0;
    std::cin >> n;

    // A flat vector makes the storage order explicit: cell (row, col) lives at
    // row * n + col. A vector of vectors would add extra indirection and obscure
    // the cache-locality lesson.
    std::vector<int> matrix(static_cast<std::size_t>(n) * n);

    // Deterministic varied data keeps the benchmark reproducible while making
    // the checksum depend on the whole traversal.
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        matrix[i] = static_cast<int>(i % 97);
    }

    long long row_check = 0;
    long long col_check = 0;

    // The two calls perform the same amount of arithmetic; the measured gap
    // comes from the order in which addresses are requested from memory.
    long long row_us = best_row_time(matrix, n, row_check);
    long long col_us = best_column_time(matrix, n, col_check);

    std::print("row-major: {} us\n", row_us);
    std::print("column-major: {} us\n", col_us);
    std::print("checksums agree: {}\n", row_check == col_check);
}
