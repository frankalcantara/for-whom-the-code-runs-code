// cap16/03-async-future.cpp
// std::async, std::future, std::promise, and std::packaged_task.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -pthread -o async_future 03-async-future.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feasync_future.exe 03-async-future.cpp

#include <cassert>
#include <algorithm>
#include <chrono>
#include <future>
#include <numeric>
#include <print>
#include <thread>
#include <vector>

long long partial_sum(const std::vector<int>& v, int lo, int hi) {
    return std::accumulate(v.begin() + lo, v.begin() + hi, 0LL);
}

long long parallel_reduce_async(const std::vector<int>& v, int n_tasks) {
    int n = static_cast<int>(v.size());
    int chunk = (n + n_tasks - 1) / n_tasks;
    std::vector<std::future<long long>> futures;

    for (int t = 0; t < n_tasks; ++t) {
        int lo = t * chunk;
        int hi = std::min(lo + chunk, n);
        if (lo >= n) break;
        futures.push_back(std::async(std::launch::async,
                                     partial_sum, std::cref(v), lo, hi));
    }

    long long total = 0;
    for (auto& f : futures) total += f.get();
    return total;
}

void promise_example() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread worker([&prom] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        prom.set_value(42);
    });

    int result = fut.get();
    worker.join();
    assert(result == 42);
}

void packaged_task_example() {
    std::packaged_task<long long(int, int)> task([](int a, int b) {
        long long s = 0;
        for (int i = a; i <= b; ++i) s += i;
        return s;
    });

    auto fut = task.get_future();
    std::thread t{std::move(task), 1, 100};
    assert(fut.get() == 5050);
    t.join();
}

int main() {
    constexpr int N = 1'000'000;
    std::vector<int> v(N, 1);

    long long seq = std::accumulate(v.begin(), v.end(), 0LL);
    long long par = parallel_reduce_async(v, 4);
    assert(seq == N);
    assert(par == N);

    promise_example();
    packaged_task_example();

    std::print("03-async-future: all assertions passed\n");
    return 0;
}
