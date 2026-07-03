// cap16/06-parallel-prefix.cpp
// Solved exercise: parallel prefix sum over values generated lazily.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o parallel_prefix 06-parallel-prefix.cpp

#include <algorithm>
#include <cassert>
#include <coroutine>
#include <future>
#include <numeric>
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
        std::suspend_always yield_value(T v) { current = v; return {}; }
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

    std::optional<T> next() {
        if (!h || h.done()) return std::nullopt;
        h.resume();
        if (h.done()) return std::nullopt;
        return h.promise().current;
    }
};

Generator<long long> squares(int n) {
    for (long long i = 0; i < n; ++i)
        co_yield i * i;
}

std::vector<long long> materialize_squares(int n) {
    auto gen = squares(n);
    std::vector<long long> v;
    v.reserve(n);
    while (auto x = gen.next()) v.push_back(*x);
    return v;
}

std::vector<long long> parallel_prefix_sum(const std::vector<long long>& v, int tasks) {
    int n = static_cast<int>(v.size());
    if (n == 0) return {};
    tasks = std::max(1, std::min(tasks, n));
    int chunk = (n + tasks - 1) / tasks;

    std::vector<long long> block_sum(tasks, 0);
    std::vector<std::future<long long>> futures;
    for (int t = 0; t < tasks; ++t) {
        int lo = t * chunk;
        int hi = std::min(lo + chunk, n);
        futures.push_back(std::async(std::launch::async, [&, lo, hi] {
            return std::accumulate(v.begin() + lo, v.begin() + hi, 0LL);
        }));
    }
    for (int t = 0; t < tasks; ++t) block_sum[t] = futures[t].get();

    std::vector<long long> offset(tasks + 1, 0);
    for (int t = 0; t < tasks; ++t)
        offset[t + 1] = offset[t] + block_sum[t];

    std::vector<long long> prefix(n);
    futures.clear();
    for (int t = 0; t < tasks; ++t) {
        int lo = t * chunk;
        int hi = std::min(lo + chunk, n);
        futures.push_back(std::async(std::launch::async, [&, lo, hi, t] {
            long long acc = offset[t];
            for (int i = lo; i < hi; ++i) {
                acc += v[i];
                prefix[i] = acc;
            }
            return 0LL;
        }));
    }
    for (auto& f : futures) f.get();
    return prefix;
}

int main() {
    auto v = materialize_squares(100);
    auto pref = parallel_prefix_sum(v, 4);
    assert(pref.back() == 99LL * 100 * 199 / 6);

    auto empty = parallel_prefix_sum({}, 4);
    assert(empty.empty());

    std::print("06-parallel-prefix: all assertions passed\n");
    return 0;
}
