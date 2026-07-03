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

    int rows = 0;
    int columns = 0;
    int blocked_row = 0;
    int blocked_column = 0;
    std::cin >> rows >> columns >> blocked_row >> blocked_column;

    const int maximum = rows + columns;
    std::vector<std::int64_t> factorial(
        static_cast<std::size_t>(maximum + 1), 1);
    std::vector<std::int64_t> inverse_factorial(
        static_cast<std::size_t>(maximum + 1), 1);

    for (int value = 1; value <= maximum; ++value) {
        factorial[static_cast<std::size_t>(value)] =
            factorial[static_cast<std::size_t>(value - 1)] * value % modulus;
    }
    inverse_factorial.back() = power_mod(factorial.back(), modulus - 2);
    for (int value = maximum; value > 0; --value) {
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

    const std::int64_t all_paths =
        choose(rows + columns - 2, rows - 1);
    const std::int64_t through_block =
        choose(blocked_row + blocked_column - 2, blocked_row - 1)
        * choose(rows - blocked_row + columns - blocked_column,
                 rows - blocked_row)
        % modulus;

    std::cout << (all_paths - through_block + modulus) % modulus << '\n';
}
