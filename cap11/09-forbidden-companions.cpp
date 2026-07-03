#include <algorithm>
#include <bit>
#include <cstdint>
#include <iostream>
#include <vector>

constexpr std::int64_t modulus = 1'000'000'007;

std::int64_t power_mod(std::int64_t base, std::int64_t exponent) {
    std::int64_t result = 1;
    while (exponent != 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1LL;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int objects = 0;
    int pair_count = 0;
    int selected = 0;
    std::cin >> objects >> pair_count >> selected;

    std::vector<std::pair<int, int>> forbidden(
        static_cast<std::size_t>(pair_count));
    for (auto& [first, second] : forbidden) {
        std::cin >> first >> second;
    }

    std::vector<std::int64_t> factorial(
        static_cast<std::size_t>(objects + 1), 1);
    std::vector<std::int64_t> inverse_factorial(
        static_cast<std::size_t>(objects + 1), 1);
    for (int value = 1; value <= objects; ++value) {
        factorial[static_cast<std::size_t>(value)] =
            factorial[static_cast<std::size_t>(value - 1)] * value % modulus;
    }
    inverse_factorial.back() = power_mod(factorial.back(), modulus - 2);
    for (int value = objects; value > 0; --value) {
        inverse_factorial[static_cast<std::size_t>(value - 1)] =
            inverse_factorial[static_cast<std::size_t>(value)] * value
            % modulus;
    }

    const auto choose = [&](int n, int k) -> std::int64_t {
        if (k < 0 || k > n) {
            return 0;
        }
        return factorial[static_cast<std::size_t>(n)]
               * inverse_factorial[static_cast<std::size_t>(k)] % modulus
               * inverse_factorial[static_cast<std::size_t>(n - k)]
               % modulus;
    };

    std::int64_t answer = 0;
    const unsigned int masks = 1U << pair_count;
    for (unsigned int mask = 0; mask < masks; ++mask) {
        std::vector<int> forced;
        for (int pair = 0; pair < pair_count; ++pair) {
            if ((mask & (1U << pair)) != 0) {
                forced.push_back(
                    forbidden[static_cast<std::size_t>(pair)].first);
                forced.push_back(
                    forbidden[static_cast<std::size_t>(pair)].second);
            }
        }
        std::ranges::sort(forced);
        const int forced_count = static_cast<int>(
            std::ranges::unique(forced).begin() - forced.begin());
        const std::int64_t ways =
            choose(objects - forced_count, selected - forced_count);

        if ((std::popcount(mask) & 1) == 0) {
            answer = (answer + ways) % modulus;
        } else {
            answer = (answer - ways + modulus) % modulus;
        }
    }

    std::cout << answer << '\n';
}
