// Exercise 4.1 - The Sleeping Pipeline
// Count sensor positions inside inclusive query intervals.
// Time: O(n log n + q log n). Space: O(n).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::vector<long long> positions(n);
    for (auto& position : positions) std::cin >> position;
    std::ranges::sort(positions);

    while (q--) {
        long long left = 0;
        long long right = 0;
        std::cin >> left >> right;

        const auto first = std::ranges::lower_bound(positions, left);
        const auto after = std::ranges::upper_bound(positions, right);
        std::print("{}\n", std::ranges::distance(first, after));
    }
}
