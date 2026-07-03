// cap16/14-false-sharing.cpp
// False sharing: independent threads that update neighboring counters fight over
// the same cache line, because coherence works at cache-line granularity, not
// per variable. Padding each counter to its own line removes the contention.
// The two layouts do identical arithmetic; only their memory spacing differs.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o false-sharing 14-false-sharing.cpp

#include <atomic>
#include <chrono>
#include <new>
#include <print>
#include <thread>
#include <vector>

using Clock = std::chrono::steady_clock;

#ifdef __cpp_lib_hardware_interference_size
constexpr std::size_t kLine = std::hardware_destructive_interference_size;
#else
constexpr std::size_t kLine = 64;
#endif

struct Padded {
    alignas(kLine) std::atomic<long long> v{0};
};

int main() {
    unsigned T = std::max(2u, std::thread::hardware_concurrency());
    const long long K = 50'000'000;

    // One accessor for both layouts: a bare atomic, or the atomic inside Padded.
    auto atom = [](auto& slot) -> std::atomic<long long>& {
        if constexpr (requires { slot.v; }) return slot.v;
        else return slot;
    };

    auto run = [&](auto& slots) {
        auto t0 = Clock::now();
        std::vector<std::jthread> pool;
        for (unsigned t = 0; t < T; ++t)
            pool.emplace_back([&, t] {
                for (long long k = 0; k < K; ++k)
                    atom(slots[t]).fetch_add(1, std::memory_order_relaxed);
            });
        pool.clear();   // jthread joins on destruction
        return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
    };

    // Shared line: T atomics packed together, several per cache line.
    std::vector<std::atomic<long long>> shared(T);
    double t_shared = run(shared);

    // Padded: each atomic alone on its own cache line.
    std::vector<Padded> padded(T);
    double t_padded = run(padded);

    std::println("threads          : {}", T);
    std::println("shared cache line: {:.1f} ms", t_shared);
    std::println("padded per line  : {:.1f} ms", t_padded);
    return 0;
}
