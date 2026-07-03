#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count = 0;
    std::cin >> count;
    std::vector<int> values(static_cast<std::size_t>(count));
    for (int& value : values) {
        std::cin >> value;
    }

    const int maximum = *std::ranges::max_element(values);
    std::vector<int> smallest_factor(static_cast<std::size_t>(maximum + 1));
    std::iota(smallest_factor.begin(), smallest_factor.end(), 0);

    for (int p = 2; 1LL * p * p <= maximum; ++p) {
        if (smallest_factor[static_cast<std::size_t>(p)] != p) {
            continue;
        }
        for (int multiple = p * p; multiple <= maximum; multiple += p) {
            if (smallest_factor[static_cast<std::size_t>(multiple)]
                == multiple) {
                smallest_factor[static_cast<std::size_t>(multiple)] = p;
            }
        }
    }

    for (int index = 0; index < count; ++index) {
        int value = values[static_cast<std::size_t>(index)];
        int divisors = 1;
        while (value > 1) {
            const int prime =
                smallest_factor[static_cast<std::size_t>(value)];
            int exponent = 0;
            do {
                value /= prime;
                ++exponent;
            } while (value > 1
                     && smallest_factor[static_cast<std::size_t>(value)]
                            == prime);
            divisors *= exponent + 1;
        }
        if (index != 0) {
            std::cout << ' ';
        }
        std::cout << divisors;
    }
    std::cout << '\n';
}
