#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int queries = 0;
    std::cin >> queries;

    std::vector<int> limit(static_cast<std::size_t>(queries));
    for (int& value : limit) {
        std::cin >> value;
    }
    const int maximum = *std::ranges::max_element(limit);

    std::vector<char> prime(static_cast<std::size_t>(maximum + 1), true);
    if (maximum >= 0) {
        prime[0] = false;
    }
    if (maximum >= 1) {
        prime[1] = false;
    }
    for (int p = 2; 1LL * p * p <= maximum; ++p) {
        if (!prime[static_cast<std::size_t>(p)]) {
            continue;
        }
        for (int multiple = p * p; multiple <= maximum; multiple += p) {
            prime[static_cast<std::size_t>(multiple)] = false;
        }
    }

    std::vector<int> count(static_cast<std::size_t>(maximum + 1), 0);
    for (int value = 1; value <= maximum; ++value) {
        count[static_cast<std::size_t>(value)] =
            count[static_cast<std::size_t>(value - 1)]
            + prime[static_cast<std::size_t>(value)];
    }

    for (int value : limit) {
        std::cout << count[static_cast<std::size_t>(value)] << '\n';
    }
}
