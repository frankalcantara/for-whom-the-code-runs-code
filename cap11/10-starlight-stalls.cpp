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

    int horses = 0;
    int stalls = 0;
    std::cin >> horses >> stalls;

    int constrained = 0;
    for (int horse = 0; horse < horses; ++horse) {
        int refused = 0;
        std::cin >> refused;
        constrained += refused != 0;
    }

    std::vector<std::int64_t> factorial(
        static_cast<std::size_t>(stalls + 1), 1);
    std::vector<std::int64_t> inverse_factorial(
        static_cast<std::size_t>(stalls + 1), 1);
    for (int value = 1; value <= stalls; ++value) {
        factorial[static_cast<std::size_t>(value)] =
            factorial[static_cast<std::size_t>(value - 1)] * value % modulus;
    }
    inverse_factorial.back() = power_mod(factorial.back(), modulus - 2);
    for (int value = stalls; value > 0; --value) {
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
    const auto permutations = [&](int n, int k) -> std::int64_t {
        if (k < 0 || k > n) {
            return 0;
        }
        return factorial[static_cast<std::size_t>(n)]
               * inverse_factorial[static_cast<std::size_t>(n - k)]
               % modulus;
    };

    std::int64_t answer = 0;
    for (int violated = 0; violated <= constrained; ++violated) {
        const std::int64_t ways =
            choose(constrained, violated)
            * permutations(stalls - violated, horses - violated) % modulus;
        if ((violated & 1) == 0) {
            answer = (answer + ways) % modulus;
        } else {
            answer = (answer - ways + modulus) % modulus;
        }
    }

    std::cout << answer << '\n';
}
