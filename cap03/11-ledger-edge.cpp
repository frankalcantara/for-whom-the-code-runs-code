// Exercise 3.2 - The Ledger at the Edge
// Parse valid long long values, including LLONG_MIN and LLONG_MAX.
// Time: O(input bytes). Space: O(buffer size).

#include <climits>
#include <cstdio>
#include <print>

class FastInput {
    static constexpr std::size_t capacity = 1U << 16;
    char buffer[capacity]{};
    std::size_t position = 0;
    std::size_t length = 0;

    char next_char() {
        if (position == length) {
            length = std::fread(buffer, 1, capacity, stdin);
            position = 0;
            if (length == 0) return '\0';
        }
        return buffer[position++];
    }

public:
    long long read_long_long() {
        char ch = next_char();
        while (ch != '-' && (ch < '0' || ch > '9')) ch = next_char();

        const bool negative = ch == '-';
        if (negative) ch = next_char();

        unsigned long long magnitude = 0;
        while (ch >= '0' && ch <= '9') {
            magnitude = magnitude * 10 + static_cast<unsigned>(ch - '0');
            ch = next_char();
        }

        constexpr auto negative_limit =
            static_cast<unsigned long long>(LLONG_MAX) + 1ULL;

        if (!negative) return static_cast<long long>(magnitude);
        if (magnitude == negative_limit) return LLONG_MIN;
        return -static_cast<long long>(magnitude);
    }
};

int main() {
    FastInput input;
    const int n = static_cast<int>(input.read_long_long());

    long long minimum = LLONG_MAX;
    long long maximum = LLONG_MIN;
    unsigned long long negative_count = 0;

    for (int i = 0; i < n; ++i) {
        const long long value = input.read_long_long();
        if (value < minimum) minimum = value;
        if (value > maximum) maximum = value;
        if (value < 0) ++negative_count;
    }

    std::print("{} {} {}\n", minimum, maximum, negative_count);
}
