// Exercise 3.3 - The Signal Printer
// Print integers from 1 through n using one explicit output buffer.
// Time: O(total output bytes). Space: O(buffer size).

#include <array>
#include <charconv>
#include <cstdio>
#include <iostream>
#include <system_error>

class BufferedWriter {
    static constexpr std::size_t capacity = 1U << 16;
    std::array<char, capacity> buffer{};
    std::size_t used = 0;

public:
    ~BufferedWriter() { flush(); }

    void flush() {
        if (used == 0) return;
        std::fwrite(buffer.data(), 1, used, stdout);
        used = 0;
    }

    void write_integer(long long value) {
        std::array<char, 32> digits{};
        auto [end, error] = std::to_chars(digits.data(), digits.data() + digits.size(), value);
        if (error != std::errc{}) return;

        const auto length = static_cast<std::size_t>(end - digits.data());
        if (used + length + 1 > buffer.size()) flush();

        for (std::size_t i = 0; i < length; ++i)
            buffer[used++] = digits[i];
        buffer[used++] = '\n';
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    BufferedWriter output;
    for (int value = 1; value <= n; ++value)
        output.write_integer(value);
}
