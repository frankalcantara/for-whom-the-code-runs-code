// Exercise 2.3 - The Market of Uneven Echoes
// Compute the lower median among frequencies of distinct product codes.
// Expected time: O(n). Space: O(d), where d is the number of distinct codes.

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <unordered_map>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<long long, int> frequency;
    frequency.reserve(static_cast<std::size_t>(n));

    for (int i = 0; i < n; ++i) {
        long long code = 0;
        std::cin >> code;
        ++frequency[code];
    }

    std::vector<int> counts;
    counts.reserve(frequency.size());
    for (const auto& [code, count] : frequency) {
        static_cast<void>(code);
        counts.push_back(count);
    }

    const auto middle = (counts.size() - 1) / 2;
    std::ranges::nth_element(counts, counts.begin() + middle);
    std::print("{}\n", counts[middle]);
}
