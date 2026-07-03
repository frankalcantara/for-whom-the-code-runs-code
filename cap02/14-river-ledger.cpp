// Exercise 2.5 - The River Ledger
// Maximum contiguous gain inside one selected interval.
// Time: O(r - l + 1). Extra space: O(1).

#include <algorithm>
#include <iostream>
#include <print>
#include <span>
#include <vector>

long long best_run(std::span<const long long> values) {
    long long ending_here = values.front();
    long long best = values.front();

    for (long long value : values.subspan(1)) {
        ending_here = std::max(value, ending_here + value);
        best = std::max(best, ending_here);
    }
    return best;
}

int main() {
    int n = 0;
    int left = 0;
    int right = 0;
    std::cin >> n >> left >> right;

    std::vector<long long> changes(n);
    for (auto& change : changes) std::cin >> change;

    const auto interval = std::span<const long long>{changes}.subspan(
        static_cast<std::size_t>(left - 1),
        static_cast<std::size_t>(right - left + 1)
    );

    std::print("{}\n", best_run(interval));
}
