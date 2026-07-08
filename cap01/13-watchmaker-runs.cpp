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

    // Removing the warm-up before sorting keeps every later position tied to the
    // retained sample, not to the original experiment order.
    measured.erase(measured.begin());

    // Once ordered, all three requested statistics become fixed positions; no
    // separate min/max pass is needed for this small retained set.
    std::ranges::sort(measured);

    const auto middle = (measured.size() - 1) / 2;
    std::print("{} {} {}\n", measured.front(), measured[middle], measured.back());
}
