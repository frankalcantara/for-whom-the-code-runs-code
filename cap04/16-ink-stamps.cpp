// Exercise 4.7 - The Ink Stamps
// Cover every black run exactly with minimum overlapping fixed-width stamps.
// Time: O(rows * cols). Extra space: O(cols).

#include <iostream>
#include <print>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rows = 0;
    int cols = 0;
    int width = 0;
    std::cin >> rows >> cols >> width;

    long long stamps = 0;
    bool possible = true;

    for (int row = 0; row < rows; ++row) {
        std::string cells;
        std::cin >> cells;

        for (int left = 0; left < cols; ) {
            if (cells[left] == 'W') {
                ++left;
                continue;
            }

            int right = left;
            while (right < cols && cells[right] == 'B') ++right;
            const int length = right - left;

            if (length < width) possible = false;
            else stamps += (length + width - 1) / width;

            left = right;
        }
    }

    if (possible) std::print("{}\n", stamps);
    else          std::print("-1\n");
}
