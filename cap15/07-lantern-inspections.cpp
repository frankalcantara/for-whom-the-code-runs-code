#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<std::pair<long long, long long>> intervals(count);
    for (auto& [left, right] : intervals) std::cin >> left >> right;
    std::ranges::sort(intervals, {}, &std::pair<long long, long long>::second);

    long long last = std::numeric_limits<long long>::lowest();
    std::vector<long long> inspections;
    for (const auto [left, right] : intervals) {
        if (last < left) {
            last = right;
            inspections.push_back(last);
        }
    }

    std::cout << inspections.size() << '\n';
    for (long long position : inspections) std::cout << position << ' ';
    std::cout << '\n';
}
