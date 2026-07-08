// Exercise 1.5 - The Tiled Observatory
// Classic spelling: compare row-major and column-major traversal.
// Time: O(n^2). Space: O(n^2).

#include <chrono>
#include <iostream>
#include <limits>
#include <print>
#include <vector>

long long row_checksum(const std::vector<int>& matrix, int n) {
    long long sum = 0;

    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            // Consecutive columns consume consecutive elements, so each cache
            // line fetched from memory contributes several useful cells.
            sum += matrix[static_cast<std::size_t>(row) * n + col];
        }
    }

    return sum;
}

long long column_checksum(const std::vector<int>& matrix, int n) {
    long long sum = 0;

    for (int col = 0; col < n; ++col) {
        for (int row = 0; row < n; ++row) {
            // The mathematical visit set is the same, but the stride through the
            // flat array is n elements instead of one.
            sum += matrix[static_cast<std::size_t>(row) * n + col];
        }
    }

    return sum;
}

long long best_row_time(const std::vector<int>& matrix, int n,
                        long long& checksum) {
    long long best = std::numeric_limits<long long>::max();
    checksum = 0;

    for (int trial = 0; trial < 5; ++trial) {
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
        // traversal as a benchmark of unused work.
        checksum ^= current;
    }

    return best;
}

long long best_column_time(const std::vector<int>& matrix, int n,
                           long long& checksum) {
    long long best = std::numeric_limits<long long>::max();
    checksum = 0;

    for (int trial = 0; trial < 5; ++trial) {
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

        checksum ^= current;
    }

    return best;
}

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> matrix(static_cast<std::size_t>(n) * n);
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        matrix[i] = static_cast<int>(i % 97);
    }

    long long row_check = 0;
    long long col_check = 0;
    long long row_us = best_row_time(matrix, n, row_check);
    long long col_us = best_column_time(matrix, n, col_check);

    std::print("row-major: {} us\n", row_us);
    std::print("column-major: {} us\n", col_us);
    std::print("checksums agree: {}\n", row_check == col_check);
}
