// cap01/05-1-likelly.cpp
// Demonstrates compiler hints: [[unlikely]], std::unreachable(), and [[assume()]].
// The code compares a standard safe loop against a heavily hinted loop.
// Note: Modern branch predictors are excellent, so the performance gap 
// heavily depends on the CPU architecture and the complexity of the pipeline.
// Compile (LLVM): clang++ -std=c++23 -O2 -Wall -Wextra -o hints 05-1-likelly.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /Fehints.exe 05-1-likelly.cpp

#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include <utility> // std::unreachable

using Clock = std::chrono::steady_clock;

enum class State : uint8_t { A, B, C, D };

struct Instruction {
    // Keeping the state and value together models a stream of small operations;
    // the benchmark measures dispatch overhead rather than container lookups.
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
    default:
        // The default branch is unreachable for well-formed input, but this
        // version keeps a defensive path so no promise is made to the optimizer.
        return 0;
    }
}

static long long process_standard(const std::vector<Instruction>& stream) {
    long long total = 0;
    for (const auto& inst : stream) {
        if (inst.value == 0) {
            // The rare fallback remains an ordinary branch, so the compiler must
            // lay out code that is correct even if zeros appear often.
            total += 1;
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

    // This is a correctness promise, not a runtime check: if an invalid State
    // reaches this point, the program has undefined behavior.
    std::unreachable();
}

static long long process_hinted(const std::vector<Instruction>& stream) {
    // Promise: the vector size is a multiple of 4. The benchmark chooses n to
    // satisfy it; otherwise this hint would authorize invalid optimizations.
    [[assume(stream.size() % 4 == 0)]];

    long long total = 0;
    for (const auto& inst : stream) {
        if (inst.value == 0) [[unlikely]] {
            // Marking the fallback as unlikely biases code layout toward the
            // normal path; it must match the generated data distribution.
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

    // The result is deliberately observed through volatile so the optimizer
    // cannot replace the timed loop with "nothing uses this value".
    volatile long long sink = f();
    (void)sink;
    return std::chrono::duration<double, std::milli>(Clock::now() - t0).count();
}

int main() {
    // 40 million elements is large enough for small code-layout differences to
    // become measurable, and it is a multiple of 4 to honor the [[assume]].
    const size_t n = 40'000'000;

    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> val_dist(0, 1'000'000);
    std::uniform_int_distribution<int> state_dist(0, 3);

    std::vector<Instruction> data(n);
    for (auto& inst : data) {
        // Zero appears very rarely, making the fallback path honest data for
        // [[unlikely]] instead of a wishful annotation.
        inst.value = (val_dist(rng) < 10) ? 0 : val_dist(rng);

        // The state is always within the four enum cases, which makes the
        // std::unreachable() promise true for this generated workload.
        inst.state = static_cast<State>(state_dist(rng));
    }

    double standard = time_ms([&] { return process_standard(data); });
    double hinted = time_ms([&] { return process_hinted(data); });

    std::println("Standard processing : {:.1f} ms", standard);
    std::println("Hinted processing   : {:.1f} ms", hinted);

    return 0;
}
