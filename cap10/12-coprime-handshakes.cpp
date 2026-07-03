#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count = 0;
    std::cin >> count;

    constexpr int maximum = 1'000'000;
    std::vector<int> frequency(static_cast<std::size_t>(maximum + 1), 0);
    for (int i = 0; i < count; ++i) {
        int value = 0;
        std::cin >> value;
        ++frequency[static_cast<std::size_t>(value)];
    }

    std::vector<int> mobius(static_cast<std::size_t>(maximum + 1), 0);
    std::vector<int> primes;
    std::vector<char> composite(static_cast<std::size_t>(maximum + 1), false);
    mobius[1] = 1;

    for (int value = 2; value <= maximum; ++value) {
        if (!composite[static_cast<std::size_t>(value)]) {
            primes.push_back(value);
            mobius[static_cast<std::size_t>(value)] = -1;
        }
        for (int prime : primes) {
            if (1LL * value * prime > maximum) {
                break;
            }
            const int product = value * prime;
            composite[static_cast<std::size_t>(product)] = true;
            if (value % prime == 0) {
                mobius[static_cast<std::size_t>(product)] = 0;
                break;
            }
            mobius[static_cast<std::size_t>(product)] =
                -mobius[static_cast<std::size_t>(value)];
        }
    }

    long long answer = 0;
    for (int divisor = 1; divisor <= maximum; ++divisor) {
        if (mobius[static_cast<std::size_t>(divisor)] == 0) {
            continue;
        }
        long long divisible = 0;
        for (int multiple = divisor; multiple <= maximum;
             multiple += divisor) {
            divisible += frequency[static_cast<std::size_t>(multiple)];
        }
        answer += mobius[static_cast<std::size_t>(divisor)]
                  * divisible * (divisible - 1) / 2;
    }

    std::cout << answer << '\n';
}
