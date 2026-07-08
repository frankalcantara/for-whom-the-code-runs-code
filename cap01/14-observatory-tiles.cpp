// Exercise 1.5 - The Tiled Observatory
// Compare row-major and column-major traversal of one contiguous matrix.
// Time: O(n^2). Space: O(n^2).

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

namespace chr = std::chrono;

template <typename Fn>
auto best_time(Fn&& fn) {
    std::array<long long, 5> times{};
    long long checksum = 0;

    for (auto& elapsed : times) {
        const auto begin = chr::steady_clock::now();

        // Folding the checksum into observable state keeps the timed traversal
        // alive even under aggressive optimization.
        checksum ^= fn();
        const auto end = chr::steady_clock::now();
        elapsed = chr::duration_cast<chr::microseconds>(end - begin).count();
    }

    auto best = times[0];
    for (long long value : times) best = std::min(best, value);
    return std::pair{best, checksum};
}

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> matrix(static_cast<std::size_t>(n) * n);
    for (std::size_t i = 0; i < matrix.size(); ++i)
        matrix[i] = static_cast<int>(i % 97);

    auto row_major = [&] {
        long long sum = 0;
        for (int row = 0; row < n; ++row) {
            for (int col = 0; col < n; ++col) {
                // Consecutive columns reuse the cache line that was just fetched.
                sum += matrix[static_cast<std::size_t>(row) * n + col];
            }
        }
        return sum;
    };

    auto column_major = [&] {
        long long sum = 0;
        for (int col = 0; col < n; ++col) {
            for (int row = 0; row < n; ++row) {
                // The same index formula is used, but row jumps turn each access
                // into a stride of n elements through memory.
                sum += matrix[static_cast<std::size_t>(row) * n + col];
            }
        }
        return sum;
    };

    const auto [row_us, row_check] = best_time(row_major);
    const auto [col_us, col_check] = best_time(column_major);

    std::print("row-major: {} us\n", row_us);
    std::print("column-major: {} us\n", col_us);
    std::print("checksums agree: {}\n", row_check == col_check);
}
