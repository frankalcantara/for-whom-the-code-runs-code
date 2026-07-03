#include <algorithm>
#include <bit>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int documents;
    int seals;
    std::cin >> documents >> seals;
    std::vector<unsigned> covers(seals);
    for (unsigned& mask : covers) {
        int count;
        std::cin >> count;
        while (count-- > 0) {
            int document;
            std::cin >> document;
            mask |= 1U << (document - 1);
        }
    }

    const unsigned full = (1U << documents) - 1U;
    const int infinity = seals + 1;
    std::vector<int> dp(1U << documents, infinity);
    dp[0] = 0;
    for (unsigned mask = 0; mask <= full; ++mask) {
        if (dp[mask] == infinity) continue;
        for (unsigned seal : covers) {
            dp[mask | seal] = std::min(dp[mask | seal], dp[mask] + 1);
        }
    }
    std::cout << (dp[full] == infinity ? -1 : dp[full]) << '\n';
}
