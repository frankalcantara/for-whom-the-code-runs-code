#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int piles = 0;
    std::cin >> piles;

    std::vector<long long> prefix(static_cast<std::size_t>(piles + 1), 0);
    for (int i = 0; i < piles; ++i) {
        long long value = 0;
        std::cin >> value;
        prefix[static_cast<std::size_t>(i + 1)] =
            prefix[static_cast<std::size_t>(i)] + value;
    }

    std::vector<std::vector<long long>> best(
        static_cast<std::size_t>(piles),
        std::vector<long long>(static_cast<std::size_t>(piles), 0));
    constexpr long long infinity =
        std::numeric_limits<long long>::max() / 4;

    for (int length = 2; length <= piles; ++length) {
        for (int left = 0; left + length <= piles; ++left) {
            const int right = left + length - 1;
            const long long merged_weight =
                prefix[static_cast<std::size_t>(right + 1)]
                - prefix[static_cast<std::size_t>(left)];
            best[static_cast<std::size_t>(left)]
                [static_cast<std::size_t>(right)] = infinity;

            for (int split = left; split < right; ++split) {
                auto& answer =
                    best[static_cast<std::size_t>(left)]
                        [static_cast<std::size_t>(right)];
                answer = std::min(
                    answer,
                    best[static_cast<std::size_t>(left)]
                        [static_cast<std::size_t>(split)]
                    + best[static_cast<std::size_t>(split + 1)]
                          [static_cast<std::size_t>(right)]
                    + merged_weight);
            }
        }
    }

    std::cout << best[0][static_cast<std::size_t>(piles - 1)] << '\n';
}
