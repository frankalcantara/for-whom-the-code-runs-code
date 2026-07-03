// Exercise 1.4 - The Watchmaker's Trial Runs
// Discard warm-up and report minimum, lower median, and maximum.
// Time: O(r log r). Space: O(r).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    int runs = 0;
    std::cin >> runs;

    std::vector<long long> measured(runs);
    for (auto& value : measured) std::cin >> value;

    measured.erase(measured.begin());
    std::ranges::sort(measured);

    const auto middle = (measured.size() - 1) / 2;
    std::print("{} {} {}\n", measured.front(), measured[middle], measured.back());
}
