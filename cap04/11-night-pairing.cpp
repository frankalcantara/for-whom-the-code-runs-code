// Exercise 4.2 - The Night Pairing
// Pair all tokens while minimizing the greatest pair sum.
// Time: O(g log g). Space: O(g).

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int groups = 0;
    std::cin >> groups;

    std::vector<std::pair<long long, long long>> tokens(groups);
    for (auto& [value, count] : tokens) std::cin >> count >> value;
    std::ranges::sort(tokens);

    int left = 0;
    int right = groups - 1;
    long long answer = 0;

    while (left < right) {
        answer = std::max(answer, tokens[left].first + tokens[right].first);
        const long long paired =
            std::min(tokens[left].second, tokens[right].second);
        tokens[left].second -= paired;
        tokens[right].second -= paired;

        if (tokens[left].second == 0) ++left;
        if (tokens[right].second == 0) --right;
    }

    if (left == right && tokens[left].second > 0)
        answer = std::max(answer, 2 * tokens[left].first);

    std::print("{}\n", answer);
}
