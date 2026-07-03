// cap15/01-ad-hoc.cpp
// Classic ad hoc patterns: digit arithmetic, simple big integers, ordering tricks, and Josephus.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o adhoc 01-ad-hoc.cpp

#include <algorithm>
#include <cassert>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using i64 = long long;

int distinct_digits_factorial(int n) {
    i64 f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    std::string s = std::to_string(f);
    std::ranges::sort(s);
    auto duplicates = std::ranges::unique(s);
    return static_cast<int>(duplicates.begin() - s.begin());
}

int digit_sum_power(i64 base, int exp) {
    std::vector<int> digits = {1};
    for (int i = 0; i < exp; ++i) {
        int carry = 0;
        for (int& d : digits) {
            int val = d * static_cast<int>(base) + carry;
            d = val % 10;
            carry = val / 10;
        }
        while (carry > 0) {
            digits.push_back(carry % 10);
            carry /= 10;
        }
    }
    return std::accumulate(digits.begin(), digits.end(), 0);
}

std::string largest_number(std::vector<int> nums) {
    std::vector<std::string> strs;
    strs.reserve(nums.size());
    for (int x : nums) strs.push_back(std::to_string(x));

    std::ranges::sort(strs, [](const std::string& a, const std::string& b) {
        return a + b > b + a;
    });

    if (strs.empty() || strs[0] == "0") return "0";
    std::string result;
    for (const auto& s : strs) result += s;
    return result;
}

int trailing_zeros_factorial(int n) {
    int count = 0;
    for (long long p = 5; p <= n; p *= 5)
        count += n / p;
    return count;
}

int josephus(int n, int k) {
    int pos = 0;
    for (int i = 2; i <= n; ++i)
        pos = (pos + k) % i;
    return pos;
}

int main() {
    assert(distinct_digits_factorial(1) == 1);
    assert(distinct_digits_factorial(5) == 3);
    assert(distinct_digits_factorial(10) == 5);

    assert(digit_sum_power(2, 10) == 7);
    assert(digit_sum_power(2, 15) == 26);
    assert(digit_sum_power(10, 5) == 1);

    assert(largest_number({10, 2}) == "210");
    assert(largest_number({3, 30, 34}) == "34330");
    assert(largest_number({0, 0}) == "0");
    assert(largest_number({}) == "0");

    assert(trailing_zeros_factorial(5) == 1);
    assert(trailing_zeros_factorial(10) == 2);
    assert(trailing_zeros_factorial(25) == 6);
    assert(trailing_zeros_factorial(100) == 24);

    assert(josephus(7, 2) == 6);
    assert(josephus(1, 2) == 0);
    assert(josephus(6, 2) == 4);

    std::print("01-ad-hoc: all assertions passed\n");
    return 0;
}
