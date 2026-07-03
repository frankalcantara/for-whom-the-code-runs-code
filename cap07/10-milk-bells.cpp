#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int farms = 0;
    int rounds = 0;
    std::cin >> farms >> rounds;

    constexpr int maximum_period = 10'000;
    std::vector<long long> production_by_period(
        static_cast<std::size_t>(maximum_period + 1), 0);

    for (int i = 0; i < farms; ++i) {
        long long production = 0;
        int period = 0;
        std::cin >> production >> period;
        production_by_period[static_cast<std::size_t>(period)] += production;
    }

    std::vector<long long> collected(
        static_cast<std::size_t>(rounds + 1), 0);

    for (int period = 1; period <= maximum_period; ++period) {
        const long long production =
            production_by_period[static_cast<std::size_t>(period)];
        if (production == 0) {
            continue;
        }
        for (int round = 1; round <= rounds; round += period) {
            collected[static_cast<std::size_t>(round)] += production;
        }
    }

    for (int round = 1; round <= rounds; ++round) {
        std::cout << collected[static_cast<std::size_t>(round)] << '\n';
    }
}
