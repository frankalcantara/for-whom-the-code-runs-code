// Exercise 4.5 - The Redundant Watch
// Determine whether one interval is contained in another.
// Time: O(n log n). Space: O(n).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<std::pair<long long, long long>> watches(n);
    for (auto& [start, end] : watches) std::cin >> start >> end;

    std::ranges::sort(watches, [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second > b.second;
    });

    long long farthest_end = watches.front().second;
    bool redundant = false;
    for (int i = 1; i < n; ++i) {
        if (watches[i].second <= farthest_end) {
            redundant = true;
            break;
        }
        farthest_end = watches[i].second;
    }

    std::print("{}\n", redundant ? "YES" : "NO");
}
