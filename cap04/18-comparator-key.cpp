// cap04/18-comparator-key.cpp
// Branchy comparator vs a single encoded integer key when sorting by a
// composite order (score descending, then id ascending). Both are O(n log n).
// Measured finding: using the key as a projection is only slightly faster than
// the branchy comparator; the real win comes from materializing the encoded
// keys into a contiguous array and sorting that array directly.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o comparator-key 18-comparator-key.cpp

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

struct Contestant { int score; int id; };

template <class F>
static double time_ms(F&& f) {
    auto t0 = Clock::now();
    f();
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    const int n = 5'000'000;
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> ds(0, (1 << 20) - 1);  // score, id < 2^20
    std::vector<Contestant> base(n);
    for (auto& c : base) c = {ds(rng), ds(rng)};

    // (a) branchy comparator: score descending, ties by id ascending.
    {
        auto v = base;
        double t = time_ms([&] {
            std::ranges::sort(v, [](const Contestant& a, const Contestant& b) {
                if (a.score != b.score) return a.score > b.score;
                return a.id < b.id;
            });
        });
        std::println("branchy comparator      : {:.1f} ms", t);
    }

    // (b) encoded key as a projection, computed during each comparison.
    {
        auto v = base;
        auto key = [](const Contestant& c) -> long long {
            long long inv = ((1LL << 20) - 1) - c.score;   // descending
            return (inv << 20) | c.id;
        };
        double t = time_ms([&] { std::ranges::sort(v, {}, key); });
        std::println("encoded-key projection  : {:.1f} ms", t);
    }

    // (c) encode every record into one integer and sort that array directly:
    // contiguous data, a plain integer sort, no comparator and no indirection.
    {
        auto v = base;
        std::vector<long long> keys(n);
        double t = time_ms([&] {
            for (int i = 0; i < n; ++i)
                keys[i] = ((((1LL << 20) - 1) - v[i].score) << 20) | v[i].id;
            std::ranges::sort(keys);   // each element carries score and id
        });
        std::println("encoded keys, sort array: {:.1f} ms", t);
    }
    return 0;
}
