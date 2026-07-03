#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<std::vector<long long>> cost(n, std::vector<long long>(n));
    for (auto& row : cost) {
        for (long long& value : row) {
            std::cin >> value;
        }
    }

    constexpr long long infinity = std::numeric_limits<long long>::max() / 4;
    const unsigned full = (1U << n) - 1U;
    std::vector<long long> dp(static_cast<std::size_t>(1U << n) * n, infinity);
    auto state = [n](unsigned mask, int last) {
        return static_cast<std::size_t>(mask) * n + last;
    };
    dp[state(1U, 0)] = 0;

    for (unsigned mask = 1; mask <= full; ++mask) {
        for (int last = 0; last < n; ++last) {
            const long long current = dp[state(mask, last)];
            if (current == infinity) continue;
            for (int next = 0; next < n; ++next) {
                if (mask & (1U << next)) continue;
                const unsigned extended = mask | (1U << next);
                dp[state(extended, next)] =
                    std::min(dp[state(extended, next)], current + cost[last][next]);
            }
        }
    }

    long long answer = infinity;
    for (int last = 1; last < n; ++last) {
        answer = std::min(answer, dp[state(full, last)] + cost[last][0]);
    }
    std::cout << (n == 1 ? 0 : answer) << '\n';
}
