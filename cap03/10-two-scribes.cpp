// Exercise 3.1 - The Two Scribes
// Run with "synced" or "fast" and feed the same input file to both runs.
// Time: O(n). Extra space: O(1).

#include <chrono>
#include <iostream>
#include <print>
#include <string_view>

int main(int argc, char* argv[]) {
    const std::string_view mode = argc > 1 ? argv[1] : "fast";
    if (mode == "fast") {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);
    }

    const auto begin = std::chrono::steady_clock::now();

    int n = 0;
    std::cin >> n;
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        sum += value;
    }

    const auto end = std::chrono::steady_clock::now();
    const auto microseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    std::print("sum={} time_us={}\n", sum, microseconds);
}
