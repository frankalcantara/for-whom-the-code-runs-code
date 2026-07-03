// Exercise 3.4 - The Weather Archive
// Read exactly n measurements from a named file with explicit diagnostics.
// Time: O(n). Space: O(1), excluding stream buffers.

#include <expected>
#include <fstream>
#include <limits>
#include <print>
#include <string>

struct Stats {
    long long sum = 0;
    long long minimum = std::numeric_limits<long long>::max();
    long long maximum = std::numeric_limits<long long>::min();
};

std::expected<Stats, std::string> read_stats(const std::string& path) {
    std::ifstream input(path);
    if (!input) return std::unexpected("cannot open file");

    int n = 0;
    if (!(input >> n)) return std::unexpected("missing measurement count");
    if (n <= 0) return std::unexpected("measurement count must be positive");

    Stats stats;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        if (!(input >> value))
            return std::unexpected("missing or malformed measurement at index " +
                                   std::to_string(i));
        stats.sum += value;
        if (value < stats.minimum) stats.minimum = value;
        if (value > stats.maximum) stats.maximum = value;
    }

    std::string extra;
    if (input >> extra) return std::unexpected("unexpected data after measurements");
    return stats;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::print("usage: {} <weather-file>\n", argv[0]);
        return 1;
    }

    const auto result = read_stats(argv[1]);
    if (!result) {
        std::print("ERROR: {}\n", result.error());
        return 1;
    }

    std::print("{} {} {}\n", result->sum, result->minimum, result->maximum);
}
