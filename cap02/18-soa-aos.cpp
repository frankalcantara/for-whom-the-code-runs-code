// cap02/18-soa-aos.cpp
// Array of structs (AoS) vs struct of arrays (SoA). Two access patterns are
// timed: sweeping a single field across all records, and touching every field
// of every record. SoA wins the single-field sweep because the wanted values
// are contiguous; AoS is competitive when whole records are used, because a
// record's fields share a cache line.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o soa-aos 18-soa-aos.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fesoa-aos.exe 18-soa-aos.cpp

#include <chrono>
#include <cstdint>
#include <print>
#include <vector>

using Clock = std::chrono::steady_clock;

struct P { int x, y, w; };               // 12 bytes per record

template <class F>
static double time_ms(F&& f) {
    auto t0 = Clock::now();
    volatile long long sink = f();        // volatile keeps the work alive
    (void)sink;
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    const int n = 30'000'000;

    std::vector<P> aos(n);
    struct { std::vector<int> x, y, w; } soa;
    soa.x.resize(n); soa.y.resize(n); soa.w.resize(n);
    for (int i = 0; i < n; ++i) {
        int x = i * 2 + 1, y = i ^ 7, w = i % 100;
        aos[i] = {x, y, w};
        soa.x[i] = x; soa.y[i] = y; soa.w[i] = w;
    }

    // Single-field sweep: sum only x.
    double aos_x = time_ms([&] {
        long long s = 0;
        for (const P& p : aos) s += p.x;          // strides by 12 bytes
        return s;
    });
    double soa_x = time_ms([&] {
        long long s = 0;
        for (int v : soa.x) s += v;               // contiguous x stream
        return s;
    });

    // Whole-record sweep: sum x + y + w.
    double aos_all = time_ms([&] {
        long long s = 0;
        for (const P& p : aos) s += p.x + p.y + p.w;
        return s;
    });
    double soa_all = time_ms([&] {
        long long s = 0;
        for (int i = 0; i < n; ++i) s += soa.x[i] + soa.y[i] + soa.w[i];
        return s;
    });

    std::println("single field  AoS: {:.1f} ms   SoA: {:.1f} ms", aos_x, soa_x);
    std::println("whole record  AoS: {:.1f} ms   SoA: {:.1f} ms", aos_all, soa_all);
    return 0;
}
