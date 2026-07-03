#include <algorithm>
#include <array>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rounds = 0;
    std::cin >> rounds;

    std::array<long long, 3> previous{};
    for (int round = 0; round < rounds; ++round) {
        std::array<long long, 3> score{};
        std::cin >> score[0] >> score[1] >> score[2];

        if (round == 0) {
            previous = score;
            continue;
        }

        std::array<long long, 3> current{};
        current[0] = score[0] + std::max(previous[1], previous[2]);
        current[1] = score[1] + std::max(previous[0], previous[2]);
        current[2] = score[2] + std::max(previous[0], previous[1]);
        previous = current;
    }

    std::cout << std::ranges::max(previous) << '\n';
}
