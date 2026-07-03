// Exercise 4.6 - The Festival Halls
// Maximize the number of demands assigned to distinct sufficient capacities.
// Time: O((n + k) log(n + k)). Space: O(n + k).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int halls_count = 0;
    int concerts_count = 0;
    std::cin >> halls_count >> concerts_count;

    std::vector<long long> halls(halls_count);
    std::vector<long long> demands(concerts_count);
    for (auto& capacity : halls) std::cin >> capacity;
    for (auto& demand : demands) std::cin >> demand;

    std::ranges::sort(halls);
    std::ranges::sort(demands);

    int hall = 0;
    int matched = 0;
    for (long long demand : demands) {
        while (hall < halls_count && halls[hall] < demand) ++hall;
        if (hall == halls_count) break;
        ++matched;
        ++hall;
    }

    std::print("{}\n", matched);
}
