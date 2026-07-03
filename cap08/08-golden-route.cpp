#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rows = 0;
    int columns = 0;
    std::cin >> rows >> columns;

    constexpr long long unreachable =
        std::numeric_limits<long long>::lowest() / 4;
    std::vector<long long> best(static_cast<std::size_t>(columns), unreachable);

    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            long long value = 0;
            std::cin >> value;

            if (row == 0 && column == 0) {
                best[0] = value;
                continue;
            }

            const long long from_above =
                best[static_cast<std::size_t>(column)];
            const long long from_left =
                column == 0
                    ? unreachable
                    : best[static_cast<std::size_t>(column - 1)];
            best[static_cast<std::size_t>(column)] =
                value + std::max(from_above, from_left);
        }
    }

    std::cout << best.back() << '\n';
}
