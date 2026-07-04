// Exercise T16.7 - The Sequence Metronome
// Session: T16.S4, exercise 3 of 3 (2 hours for all three)
// Theme: chapter 16 (coroutine generator)
// Tags: chapter-16
//
// MODEL: generate an arithmetic sequence lazily and sum the yielded
//   values.
// MATH: the generator yields a + i*d for i = 0..n-1. The consumer
//   accumulates each yielded value exactly once.
// PROOF: loop invariant. Before the i-th suspension, current holds
//   a + i*d. co_yield exposes that value to the consumer, then the
//   coroutine advances current by d. After n yields the coroutine
//   completes, so the consumer has seen exactly the requested
//   sequence.
// COMPLEXITY: O(n) time and O(1) generator state. A closed-form sum
//   would be faster, but the exercise is about lazy production.
// TYPES: with n <= 1e6 and |a|, |d| <= 1e6, the largest absolute
//   sum is below 5.1e17, so int64_t is sufficient.
// ALTERNATIVES: a simple for loop is the right production choice
//   when no suspension behavior is needed; the closed form is best
//   when only the sum matters.
// EDGE CASES: n = 0; negative d; negative a; large n with large
//   values.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o metronome t16-s4-3-sequence-metronome.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femetronome.exe t16-s4-3-sequence-metronome.cpp

#include <coroutine>
#include <cstdint>
#include <iostream>

class Generator {
   public:
    struct promise_type {
        std::int64_t value = 0;

        Generator get_return_object() {
            return Generator{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::int64_t v) noexcept {
            value = v;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { throw; }
    };

    explicit Generator(std::coroutine_handle<promise_type> handle)
        : handle_(handle) {}

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other) noexcept : handle_(other.handle_) {
        other.handle_ = {};
    }

    ~Generator() {
        if (handle_) handle_.destroy();
    }

    bool next() {
        handle_.resume();
        return !handle_.done();
    }

    std::int64_t value() const { return handle_.promise().value; }

   private:
    std::coroutine_handle<promise_type> handle_;
};

Generator arithmetic_sequence(int n, std::int64_t start, std::int64_t step) {
    std::int64_t current = start;
    for (int i = 0; i < n; ++i) {
        co_yield current;
        current += step;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::int64_t start = 0;
    std::int64_t step = 0;
    std::cin >> n >> start >> step;

    std::int64_t sum = 0;
    auto generator = arithmetic_sequence(n, start, step);
    while (generator.next()) {
        sum += generator.value();
    }

    std::cout << sum << '\n';
    return 0;
}
