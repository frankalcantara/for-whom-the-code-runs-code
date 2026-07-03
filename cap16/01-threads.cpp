// cap16/01-threads.cpp
// std::thread, std::jthread, atomics, and the shape of a data race.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o threads 01-threads.cpp

#include <atomic>
#include <cassert>
#include <chrono>
#include <print>
#include <thread>
#include <vector>

long long race_condition(int n_threads, int increments_each) {
    long long counter = 0;
    std::vector<std::thread> threads;
    threads.reserve(n_threads);

    for (int t = 0; t < n_threads; ++t) {
        threads.emplace_back([&counter, increments_each] {
            for (int i = 0; i < increments_each; ++i)
                ++counter; // intentional data race: load, add, store are not atomic
        });
    }

    for (auto& th : threads) th.join();
    return counter;
}

long long atomic_counter(int n_threads, int increments_each) {
    std::atomic<long long> counter{0};
    std::vector<std::thread> threads;
    threads.reserve(n_threads);

    for (int t = 0; t < n_threads; ++t) {
        threads.emplace_back([&counter, increments_each] {
            for (int i = 0; i < increments_each; ++i)
                counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    for (auto& th : threads) th.join();
    return counter.load(std::memory_order_relaxed);
}

void jthread_example() {
    std::atomic<int> count{0};

    {
        std::jthread worker([&count](std::stop_token st) {
            while (!st.stop_requested()) {
                count.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        worker.request_stop();
    }

    assert(count.load(std::memory_order_relaxed) > 0);
}

int main() {
    constexpr int T = 4;
    constexpr int N = 100'000;
    long long expected = static_cast<long long>(T) * N;

    long long racy = race_condition(T, N);
    std::print("race_condition: {} observed, {} expected\n", racy, expected);

    long long safe = atomic_counter(T, N);
    assert(safe == expected);

    jthread_example();

    std::print("01-threads: all assertions passed\n");
    return 0;
}
