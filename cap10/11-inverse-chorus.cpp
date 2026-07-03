#include <cstdint>
#include <iostream>
#include <vector>

std::uint64_t power_mod(std::uint64_t base,
                        std::uint64_t exponent,
                        std::uint64_t modulus) {
    std::uint64_t result = 1;
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
    int count = 0;
    std::cin >> prime >> count;

    std::vector<std::uint64_t> value(static_cast<std::size_t>(count));
    std::vector<std::uint64_t> prefix(
        static_cast<std::size_t>(count + 1), 1);

    for (int i = 0; i < count; ++i) {
        std::cin >> value[static_cast<std::size_t>(i)];
        prefix[static_cast<std::size_t>(i + 1)] =
            prefix[static_cast<std::size_t>(i)]
            * value[static_cast<std::size_t>(i)] % prime;
    }

    std::uint64_t suffix_inverse =
        power_mod(prefix.back(), prime - 2, prime);
    std::uint64_t answer = 0;

    for (int i = count - 1; i >= 0; --i) {
        const std::uint64_t inverse =
            prefix[static_cast<std::size_t>(i)] * suffix_inverse % prime;
        answer = (answer + inverse) % prime;
        suffix_inverse =
            suffix_inverse * value[static_cast<std::size_t>(i)] % prime;
    }

    std::cout << answer << '\n';
}
