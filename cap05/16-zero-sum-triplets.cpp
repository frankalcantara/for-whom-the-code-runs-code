#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::vector<long long> runes(static_cast<std::size_t>(n));
    for (long long& rune : runes) {
        std::cin >> rune;
    }

    std::ranges::sort(runes);

    long long answer = 0;
    for (int first = 0; first + 2 < n; ++first) {
        int left = first + 1;
        int right = n - 1;

        while (left < right) {
            const long long sum =
                runes[static_cast<std::size_t>(first)]
                + runes[static_cast<std::size_t>(left)]
                + runes[static_cast<std::size_t>(right)];

            if (sum < 0) {
                ++left;
            } else if (sum > 0) {
                --right;
            } else if (runes[static_cast<std::size_t>(left)]
                       == runes[static_cast<std::size_t>(right)]) {
                const long long count = right - left + 1LL;
                answer += count * (count - 1) / 2;
                break;
            } else {
                const long long left_value =
                    runes[static_cast<std::size_t>(left)];
                const long long right_value =
                    runes[static_cast<std::size_t>(right)];

                long long left_count = 0;
                long long right_count = 0;
                while (left < right
                       && runes[static_cast<std::size_t>(left)]
                              == left_value) {
                    ++left_count;
                    ++left;
                }
                while (right >= left
                       && runes[static_cast<std::size_t>(right)]
                              == right_value) {
                    ++right_count;
                    --right;
                }
                answer += left_count * right_count;
            }
        }
    }

    std::cout << answer << '\n';
}
