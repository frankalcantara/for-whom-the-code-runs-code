// Exercise 4.4 - The Copyist's Error
// Find the duplicated and missing labels from 1 through n.
// Time: O(n log n). Space: O(n).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<long long> labels(n);
    long long actual_sum = 0;
    for (auto& label : labels) {
        std::cin >> label;
        actual_sum += label;
    }

    std::ranges::sort(labels);

    long long duplicate = -1;
    for (int i = 1; i < n; ++i)
        if (labels[i] == labels[i - 1]) duplicate = labels[i];

    const long long expected_sum = 1LL * n * (n + 1) / 2;
    const long long missing = expected_sum - actual_sum + duplicate;
    std::print("{} {}\n", duplicate, missing);
}
