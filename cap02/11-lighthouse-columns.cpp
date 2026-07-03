// Exercise 2.2 - The Lighthouse Window
// Find the column with maximum total light.
// Time: O(rows * cols). Space: O(cols).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    int rows = 0;
    int cols = 0;
    std::cin >> rows >> cols;

    std::vector<long long> column_sum(cols, 0);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            long long light = 0;
            std::cin >> light;
            column_sum[col] += light;
        }
    }

    const auto best = std::ranges::max_element(column_sum);
    const auto column = std::ranges::distance(column_sum.begin(), best);
    std::print("{} {}\n", column + 1, *best);
}
