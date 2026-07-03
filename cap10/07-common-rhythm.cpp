#include <iostream>
#include <numeric>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int values = 0;
    std::cin >> values;

    long long common = 0;
    for (int i = 0; i < values; ++i) {
        long long value = 0;
        std::cin >> value;
        common = std::gcd(common, value);
    }

    std::cout << common << '\n';
}
