// cap16/05-coroutines.cpp
// A small coroutine generator for lazy sequences.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o coroutines 05-coroutines.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Fecoroutines.exe 05-coroutines.cpp

#include <cassert>
#include <coroutine>
#include <optional>
#include <print>
#include <vector>

template<typename T>
struct Generator {
    struct promise_type {
        std::optional<T> current{};

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T v) {
            current = v;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { throw; }
    };

    using Handle = std::coroutine_handle<promise_type>;
    Handle h{};

    explicit Generator(Handle h_) : h(h_) {}
    ~Generator() { if (h) h.destroy(); }

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : h(other.h) { other.h = {}; }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (h) h.destroy();
            h = other.h;
            other.h = {};
        }
        return *this;
    }

    std::optional<T> next() {
        if (!h || h.done()) return std::nullopt;
        h.resume();
        if (h.done()) return std::nullopt;
        return h.promise().current;
    }
};

Generator<long long> fibonacci() {
    long long a = 0, b = 1;
    while (true) {
        co_yield a;
        auto c = a + b;
        a = b;
        b = c;
    }
}

Generator<int> range(int lo, int hi, int step = 1) {
    for (int i = lo; i < hi; i += step)
        co_yield i;
}

int main() {
    auto fib = fibonacci();
    std::vector<long long> got;
    for (int i = 0; i < 10; ++i)
        got.push_back(fib.next().value());
    assert((got == std::vector<long long>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34}));

    auto r = range(0, 10, 2);
    std::vector<int> vals;
    while (auto v = r.next()) vals.push_back(*v);
    assert((vals == std::vector<int>{0, 2, 4, 6, 8}));

    std::print("05-coroutines: all assertions passed\n");
    return 0;
}
