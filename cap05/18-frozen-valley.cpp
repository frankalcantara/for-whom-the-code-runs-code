#include <algorithm>
#include <bit>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int queries = 0;
    std::cin >> n >> queries;

    const int levels = static_cast<int>(
        std::bit_width(static_cast<unsigned int>(n)));
    std::vector<std::vector<long long>> table(
        static_cast<std::size_t>(levels),
        std::vector<long long>(static_cast<std::size_t>(n)));

    for (long long& value : table[0]) {
        std::cin >> value;
    }

    for (int level = 1; level < levels; ++level) {
        const int length = 1 << level;
        const int half = length >> 1;
        for (int start = 0; start + length <= n; ++start) {
            table[static_cast<std::size_t>(level)]
                 [static_cast<std::size_t>(start)] =
                std::min(
                    table[static_cast<std::size_t>(level - 1)]
                         [static_cast<std::size_t>(start)],
                    table[static_cast<std::size_t>(level - 1)]
                         [static_cast<std::size_t>(start + half)]);
        }
    }

    while (queries-- > 0) {
        int left = 0;
        int right = 0;
        std::cin >> left >> right;
        --left;
        --right;

        const unsigned int length =
            static_cast<unsigned int>(right - left + 1);
        const int level =
            static_cast<int>(std::bit_width(length) - 1U);
        const int block_length = 1 << level;

        const long long answer = std::min(
            table[static_cast<std::size_t>(level)]
                 [static_cast<std::size_t>(left)],
            table[static_cast<std::size_t>(level)]
                 [static_cast<std::size_t>(right - block_length + 1)]);
        std::cout << answer << '\n';
    }
}
