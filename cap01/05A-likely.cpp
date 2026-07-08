// cap01/06-compiler-hints.cpp
// Demonstrates compiler hints: [[unlikely]], std::unreachable(), and [[assume()]].
// The code compares a standard safe loop against a heavily hinted loop.
// Note: Modern branch predictors are excellent, so the performance gap 
// heavily depends on the CPU architecture and the complexity of the pipeline.
// Compile (LLVM): clang++ -std=c++23 -O2 -Wall -Wextra -o hints 06-compiler-hints.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /Fehints.exe 06-compiler-hints.cpp

#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include <utility> // std::unreachable

using Clock = std::chrono::steady_clock;

enum class State : uint8_t { A, B, C, D };

struct Instruction {
    State state;
    int value;
};

// Standard implementation: safe, handles defaults, compiler must insert checks.
static int handle_standard(State s, int val) {
    switch (s) {
    case State::A: return val + 1;
    case State::B: return val - 1;
    case State::C: return val * 2;
    case State::D: return val / 2;
    default: return 0; // Compiler must generate code for this branch
    }
}

static long long process_standard(const std::vector<Instruction>& stream) {
    long long total = 0;
    for (const auto& inst : stream) {
        if (inst.value == 0) {
            total += 1; // error handling or rare fallback
            continue;
        }
        total += handle_standard(inst.state, inst.value);
    }
    return total;
}

// Hinted implementation: aggressive promises to the compiler.
static int handle_hinted(State s, int val) {
    switch (s) {
    case State::A: return val + 1;
    case State::B: return val - 1;
    case State::C: return val * 2;
    case State::D: return val / 2;
    }
    std::unreachable(); // Promise: no other State exists. Drops the default branch.
}

static long long process_hinted(const std::vector<Instruction>& stream) {
    // Promise: the vector size is a multiple of 4. 
    // This allows the optimizer to safely unroll the loop without bounds-checking the tail.
    [[assume(stream.size() % 4 == 0)]];

    long long total = 0;
    for (const auto& inst : stream) {
        if (inst.value == 0) [[unlikely]] {
            total += 1;
            continue;
        }
        total += handle_hinted(inst.state, inst.value);
    }
    return total;
}

template <class F>
static double time_ms(F&& f) {
    auto t0 = Clock::now();
    volatile long long sink = f();   // volatile keeps the work from being elided
    (void)sink;
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    // 40 million elements, explicitly a multiple of 4 to honor the [[assume]]
    const size_t n = 40'000'000;

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> val_dist(0, 1'000'000);
    std::uniform_int_distribution<int> state_dist(0, 3);

    std::vector<Instruction> data(n);
    for (auto& inst : data) {
        // Force the value to be 0 very rarely (approx 1 in 100,000)
        inst.value = (val_dist(rng) < 10) ? 0 : val_dist(rng);
        inst.state = static_cast<State>(state_dist(rng));
    }

    double standard = time_ms([&] { return process_standard(data); });
    double hinted = time_ms([&] { return process_hinted(data); });

    std::println("Standard processing : {:.1f} ms", standard);
    std::println("Hinted processing   : {:.1f} ms", hinted);

    return 0;
}