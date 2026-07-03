#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

long long exponent_in_factorial(long long n, long long prime) {
    long long exponent = 0;
    while (n > 0) {
        n /= prime;
        exponent += n;
    }
    return exponent;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n;
    long long base;
    std::cin >> n >> base;

    std::vector<std::pair<long long, int>> factors;
    for (long long divisor = 2; divisor <= base / divisor; ++divisor) {
        if (base % divisor != 0) continue;
        int exponent = 0;
        while (base % divisor == 0) {
            base /= divisor;
            ++exponent;
        }
        factors.push_back({divisor, exponent});
    }
    if (base > 1) factors.push_back({base, 1});

    long long answer = std::numeric_limits<long long>::max();
    for (const auto [prime, required] : factors) {
        answer = std::min(
            answer, exponent_in_factorial(n, prime) / required);
    }
    std::cout << answer << '\n';
}
