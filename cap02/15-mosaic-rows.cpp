// Exercise 2.6 - The Mosaic Ledger
// Store one matrix contiguously and print every row sum.
// Time: O(rows * cols). Space: O(rows * cols).

#include <iostream>
#include <print>
#include <vector>

int main() {
    int rows = 0;
    int cols = 0;
    std::cin >> rows >> cols;

    std::vector<long long> mosaic(static_cast<std::size_t>(rows) * cols);
    for (auto& value : mosaic) std::cin >> value;

    for (int row = 0; row < rows; ++row) {
        long long sum = 0;
        for (int col = 0; col < cols; ++col) {
            const auto index = static_cast<std::size_t>(row) * cols + col;
            sum += mosaic[index];
        }
        std::print("{}\n", sum);
    }
}
