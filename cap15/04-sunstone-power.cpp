#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long base;
    int exponent;
    std::cin >> base >> exponent;

    std::vector<int> digits{1};
    while (exponent-- > 0) {
        long long carry = 0;
        for (int& digit : digits) {
            const long long product = digit * base + carry;
            digit = static_cast<int>(product % 10);
            carry = product / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }

    std::cout << std::accumulate(digits.begin(), digits.end(), 0LL) << '\n';
}
