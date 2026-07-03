// cap02/08-frequency-analysis.cpp
// Complete frequency-analysis example for Chapter 02.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o frequency_analysis 08-frequency-analysis.cpp

#include <algorithm>
#include <print>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

using Vi = std::vector<int>;

struct FreqResult {
    int mode = 0;
    int mode_freq = 0;
    int singletons = 0;
    std::vector<std::pair<int, int>> table;
};

FreqResult solve_naive(const Vi& a) {
    Vi distinct;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        bool first = true;
        for (int j = 0; j < i; ++j) {
            if (a[j] == a[i]) {
                first = false;
                break;
            }
        }
        if (first) distinct.push_back(a[i]);
    }

    std::ranges::sort(distinct);

    FreqResult result;
    for (int value : distinct) {
        int count = static_cast<int>(std::ranges::count(a, value));
        result.table.push_back({value, count});
        if (count > result.mode_freq ||
            (count == result.mode_freq && value < result.mode)) {
            result.mode = value;
            result.mode_freq = count;
        }
        if (count == 1) ++result.singletons;
    }
    return result;
}

FreqResult solve_sort(Vi a) {
    std::ranges::sort(a);

    FreqResult result;
    for (int i = 0; i < static_cast<int>(a.size()); ) {
        int value = a[i];
        int count = 0;
        while (i < static_cast<int>(a.size()) && a[i] == value) {
            ++i;
            ++count;
        }

        result.table.push_back({value, count});
        if (count > result.mode_freq ||
            (count == result.mode_freq && value < result.mode)) {
            result.mode = value;
            result.mode_freq = count;
        }
        if (count == 1) ++result.singletons;
    }
    return result;
}

FreqResult solve_hash(const Vi& a) {
    std::unordered_map<int, int> freq;
    freq.reserve(a.size());
    freq.max_load_factor(0.25f);

    for (int x : a) ++freq[x];

    FreqResult result;
    result.table.reserve(freq.size());
    for (auto [value, count] : freq) {
        result.table.push_back({value, count});
        if (count > result.mode_freq ||
            (count == result.mode_freq && value < result.mode)) {
            result.mode = value;
            result.mode_freq = count;
        }
        if (count == 1) ++result.singletons;
    }

    std::ranges::sort(result.table);
    return result;
}

void print_result(const char* label, const FreqResult& result) {
    std::print("{}\n", label);
    std::print("mode: {} frequency: {}\n", result.mode, result.mode_freq);
    std::print("singletons: {}\n", result.singletons);
    std::print("table:");
    for (auto [value, count] : result.table)
        std::print(" ({}:{})", value, count);
    std::print("\n\n");
}

int main() {
    Vi data = {5, 1, 5, 2, 1, 5, 9, 2, 7};

    auto naive = solve_naive(data);
    auto sorted = solve_sort(data);
    auto hashed = solve_hash(data);

    print_result("naive", naive);
    print_result("sort", sorted);
    print_result("hash", hashed);
}
