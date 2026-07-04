// cap03/05-sequence-statistics.cpp
// Solved exercise: sequence statistics.
//
// Problem: given n integers read from stdin, compute the sum, minimum, maximum,
// and the count of strictly positive values.
//
// Input:  line 1 = n; line 2 = n integers, possibly negative.
// Output: sum min max positives
//
// Three implementations with different I/O layers:
//   Layer 1: std::cin/cout with sync enabled by default    O(n)
//   Layer 2: std::cin/cout with sync disabled              O(n), smaller constant
//   Layer 3: fread plus manual parser                      O(n), minimal overhead
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o sequence_stats 05-sequence-statistics.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesequence_stats.exe 05-sequence-statistics.cpp
// Bench:   python3 -c "import random; n=1000000; print(n); print(' '.join(str(random.randint(-10**6,10**6)) for _ in range(n)))" > /tmp/big.txt
//          time ./sequence_stats layer1 < /tmp/big.txt
//          time ./sequence_stats layer2 < /tmp/big.txt
//          time ./sequence_stats layer3 < /tmp/big.txt

#include <array>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdio>
#include <iostream>
#include <print>
#include <ranges>
#include <string_view>
#include <vector>

// ---- Shared result type ----
struct Stats {
    long long sum;
    int mn, mx, positives;
};

// ============================================================
// Layer 1 — Naive: std::cin with default sync
// Slowest: each cin >> synchronizes with C stdio.
// Time: O(n)  — constant factor ~8x larger than fread
// ============================================================
Stats solve_synced(int n) {
    // Sync is ON by default — this is how beginners typically read input.
    Stats s{0, INT_MAX, INT_MIN, 0};
    for (int i = 0; i < n; ++i) {
        int x; std::cin >> x;
        s.sum += x;
        s.mn = std::min(s.mn, x);
        s.mx = std::max(s.mx, x);
        if (x > 0) ++s.positives;
    }
    return s;
}

// ============================================================
// Layer 2 — Algorithmic: std::cin with sync disabled
// Competitive standard: two lines at top of main().
// Time: O(n) — constant factor ~3x larger than fread
// ============================================================
Stats solve_fast_cin(int n) {
    Stats s{0, INT_MAX, INT_MIN, 0};
    for (int i = 0; i < n; ++i) {
        int x; std::cin >> x;
        s.sum += x;
        s.mn = std::min(s.mn, x);
        s.mx = std::max(s.mx, x);
        if (x > 0) ++s.positives;
    }
    return s;
}

// ============================================================
// Layer 3 — High Performance: fread + manual parser
// One syscall reads all of stdin; parse directly from buffer.
// Time: O(n) — minimal syscall overhead, no stream state machine
// ============================================================
static constexpr int BUF = 1 << 23;   // 8 MB
static char g_buf[BUF];
static int  g_pos = 0, g_len = 0;

void load() { g_len = static_cast<int>(fread(g_buf, 1, BUF, stdin)); }

int read_signed() {
    while (g_pos < g_len &&
           g_buf[g_pos] != '-' && (g_buf[g_pos] < '0' || g_buf[g_pos] > '9'))
        ++g_pos;
    int sign = 1;
    if (g_pos < g_len && g_buf[g_pos] == '-') { sign = -1; ++g_pos; }
    int v = 0;
    while (g_pos < g_len && g_buf[g_pos] >= '0' && g_buf[g_pos] <= '9')
        v = v * 10 + (g_buf[g_pos++] - '0');
    return sign * v;
}

Stats solve_fread(int n) {
    Stats s{0, INT_MAX, INT_MIN, 0};
    for (int i = 0; i < n; ++i) {
        int x = read_signed();
        s.sum += x;
        s.mn = std::min(s.mn, x);
        s.mx = std::max(s.mx, x);
        if (x > 0) ++s.positives;
    }
    return s;
}

// ============================================================
// Benchmark harness — measures wall-clock time with stdin piped from file
// ============================================================
template <std::size_t Runs = 5, typename Fn>
long long bench(Fn&& fn) {
    namespace chr = std::chrono;
    std::array<long long, Runs> t{};
    for (std::size_t i = 0; i < Runs; ++i) {
        auto t0 = chr::high_resolution_clock::now();
        fn();
        t[i] = chr::duration_cast<chr::microseconds>(
            chr::high_resolution_clock::now() - t0).count();
    }
    return *std::ranges::min_element(t.begin() + 1, t.end());
}

void print_stats(const Stats& s) {
    std::print("sum={} min={} max={} positives={}\n",
               s.sum, s.mn, s.mx, s.positives);
}

int main(int argc, char* argv[]) {
    std::string_view mode = (argc > 1) ? argv[1] : "layer3";

    if (mode == "layer1") {
        // Default sync — read n then n values
        int n; std::cin >> n;
        auto s = solve_synced(n);
        print_stats(s);
    }
    else if (mode == "layer2") {
        std::ios_base::sync_with_stdio(false);  // disable sync
        std::cin.tie(nullptr);                  // stop flushing cout on cin
        int n; std::cin >> n;
        auto s = solve_fast_cin(n);
        print_stats(s);
    }
    else {
        // layer3 (default): fread
        load();
        int n = read_signed();
        auto s = solve_fread(n);
        print_stats(s);
    }
    return 0;
}
