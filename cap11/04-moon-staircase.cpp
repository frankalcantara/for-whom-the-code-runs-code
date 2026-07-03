#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr long long modulus = 1'000'000'007;
    int steps = 0;
    std::cin >> steps;

    long long two_before = 1;
    long long one_before = 1;
    for (int current = 2; current <= steps; ++current) {
        const long long ways = (one_before + two_before) % modulus;
        two_before = one_before;
        one_before = ways;
    }

    std::cout << one_before << '\n';
}
