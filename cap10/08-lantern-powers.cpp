#include <cstdint>
#include <iostream>

std::uint64_t power_mod(std::uint64_t base,
                        std::uint64_t exponent,
                        std::uint64_t modulus) {
    std::uint64_t result = 1 % modulus;
    while (exponent != 0) {
        if ((exponent & 1U) != 0) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1U;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::uint64_t prime = 0;
    int queries = 0;
    std::cin >> prime >> queries;

    while (queries-- > 0) {
        std::uint64_t base = 0;
        std::uint64_t exponent = 0;
        std::cin >> base >> exponent;
        std::cout << power_mod(base % prime, exponent, prime) << '\n';
    }
}
