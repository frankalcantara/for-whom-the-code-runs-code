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

    int symbols = 0;
    int types = 0;
    std::cin >> symbols >> types;

    std::vector<int> multiplicity(static_cast<std::size_t>(types));
    for (int& value : multiplicity) {
        std::cin >> value;
    }

    std::vector<std::int64_t> factorial(
        static_cast<std::size_t>(symbols + 1), 1);
    std::vector<std::int64_t> inverse_factorial(
        static_cast<std::size_t>(symbols + 1), 1);
    for (int value = 1; value <= symbols; ++value) {
        factorial[static_cast<std::size_t>(value)] =
            factorial[static_cast<std::size_t>(value - 1)] * value % modulus;
    }
    inverse_factorial.back() = power_mod(factorial.back(), modulus - 2);
    for (int value = symbols; value > 0; --value) {
        inverse_factorial[static_cast<std::size_t>(value - 1)] =
            inverse_factorial[static_cast<std::size_t>(value)] * value
            % modulus;
    }

    std::int64_t answer = factorial.back();
    for (int count : multiplicity) {
        answer = answer
                 * inverse_factorial[static_cast<std::size_t>(count)]
                 % modulus;
    }
    std::cout << answer << '\n';
}
