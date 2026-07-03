#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr long long modulus = 1'000'000'007;
    int people = 0;
    std::cin >> people;

    if (people == 0) {
        std::cout << 1 << '\n';
        return 0;
    }

    long long two_before = 1;
    long long one_before = 0;
    for (int current = 2; current <= people; ++current) {
        const long long ways =
            (current - 1LL) * (one_before + two_before) % modulus;
        two_before = one_before;
        one_before = ways;
    }
    std::cout << one_before << '\n';
}
