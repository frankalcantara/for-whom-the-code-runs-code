// cap07/14-generator-benchmark.cpp
// Microbenchmark: C++23 std::generator versus a recursive callback fallback
// for lazy k-combination traversal.
//
// Compile with MSVC:
//   cl /nologo /EHsc /O2 /W4 /permissive- /Zc:__cplusplus /std:c++latest 14-generator-benchmark.cpp

#include <cassert>
#include <chrono>
#include <cstdint>
#include <generator>
#include <print>
#include <span>
#include <vector>

using Clock = std::chrono::steady_clock;

struct Result {
    std::uint64_t count = 0;
    std::uint64_t checksum = 0;

    friend bool operator==(const Result&, const Result&) = default;
};

std::generator<std::span<const int>> generated_combinations(int n, int k) {
    std::vector<int> c(k);
    for (int i = 0; i < k; ++i) c[i] = i;
    while (true) {
        co_yield std::span<const int>(c);
        int i = k - 1;
        while (i >= 0 && c[i] == n - k + i) --i;
        if (i < 0) co_return;
        ++c[i];
        for (int j = i + 1; j < k; ++j) c[j] = c[j - 1] + 1;
    }
}

static std::uint64_t score(std::span<const int> comb) {
    std::uint64_t value = 1469598103934665603ULL;
    for (int x : comb) {
        value ^= static_cast<std::uint64_t>(x + 1);
        value *= 1099511628211ULL;
    }
    return value;
}

Result consume_generator(int n, int k) {
    Result result;
    for (std::span<const int> comb : generated_combinations(n, k)) {
        ++result.count;
        result.checksum += score(comb);
    }
    return result;
}

template <class Fn>
void callback_combinations_rec(int n, int k, int next, std::vector<int>& current,
                               Fn&& fn) {
    if (static_cast<int>(current.size()) == k) {
        fn(std::span<const int>(current));
        return;
    }

    const int need = k - static_cast<int>(current.size());
    for (int x = next; x <= n - need; ++x) {
        current.push_back(x);
        callback_combinations_rec(n, k, x + 1, current, fn);
        current.pop_back();
    }
}

Result consume_callback(int n, int k) {
    Result result;
    std::vector<int> current;
    current.reserve(k);
    callback_combinations_rec(n, k, 0, current, [&](std::span<const int> comb) {
        ++result.count;
        result.checksum += score(comb);
    });
    return result;
}

template <class Fn>
double best_ms(Fn&& fn) {
    double best = 1e100;
    for (int run = 0; run < 7; ++run) {
        const auto t0 = Clock::now();
        Result result = fn();
        const auto t1 = Clock::now();
        volatile std::uint64_t keep_alive = result.checksum + result.count;
        (void)keep_alive;
        const double ms =
            std::chrono::duration<double, std::milli>(t1 - t0).count();
        if (run > 0 && ms < best) best = ms;
    }
    return best;
}

int main() {
    constexpr int n = 30;
    constexpr int k = 6;

    const Result generator_result = consume_generator(n, k);
    const Result callback_result = consume_callback(n, k);
    assert(generator_result == callback_result);

    const double generator_ms = best_ms([&] { return consume_generator(n, k); });
    const double callback_ms = best_ms([&] { return consume_callback(n, k); });

    std::println("n={}, k={}, combinations={}", n, k, generator_result.count);
    std::println("std::generator      : {:.2f} ms", generator_ms);
    std::println("recursive callback  : {:.2f} ms", callback_ms);
    std::println("checksum            : {}", generator_result.checksum);
}
