// Exercise T16.4 - The Workshop Queue
// Session: T16.S3, exercise 2 of 2 (1 hour for both)
// Theme: chapter 16 (condition_variable producer-consumer queue)
// Tags: chapter-16, parallelism
//
// MODEL: a shared queue holds ticket costs. Worker threads sleep
//   until either work exists or production is finished.
// MATH: every ticket is pushed once and popped once. A worker adds
//   popped costs to a private local sum, then atomically publishes
//   that sum to the global total.
// PROOF: queue invariant. The mutex protects the queue and done
//   flag, so no two workers pop the same ticket. The condition
//   variable wait uses a predicate, therefore spurious wakeups only
//   re-check the state. Once done is true and the queue is empty,
//   every pushed ticket has been processed.
// COMPLEXITY: O(n) total work plus synchronization overhead.
//   Memory is O(n + p) for queued work and workers.
// TYPES: total cost can reach 2e14 in magnitude, so int64_t is used.
// ALTERNATIVES: if all work is already in memory, static block
//   partitioning is cheaper; the queue pattern is for dynamic work
//   arrival.
// EDGE CASES: n = 0; p > n; negative costs; spurious wakeups.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o queue t16-s3-2-workshop-queue.cpp

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <print>
#include <queue>
#include <thread>
#include <vector>

struct Channel {
    std::queue<std::int64_t> jobs;
    std::mutex mutex;
    std::condition_variable cv;
    bool done = false;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int workers = 0;
    std::cin >> n >> workers;

    Channel channel;
    std::atomic<std::int64_t> total{0};
    std::vector<std::jthread> pool;
    pool.reserve(static_cast<std::size_t>(workers));
    for (int id = 0; id < workers; ++id) {
        pool.emplace_back([&] {
            std::int64_t local = 0;
            while (true) {
                std::unique_lock lock(channel.mutex);
                channel.cv.wait(lock, [&] {
                    return channel.done || !channel.jobs.empty();
                });
                if (channel.jobs.empty()) {
                    if (channel.done) break;
                    continue;
                }
                const std::int64_t job = channel.jobs.front();
                channel.jobs.pop();
                lock.unlock();
                local += job;
            }
            total.fetch_add(local, std::memory_order_relaxed);
        });
    }

    {
        std::scoped_lock lock(channel.mutex);
        for (int i = 0; i < n; ++i) {
            std::int64_t cost = 0;
            std::cin >> cost;
            channel.jobs.push(cost);
        }
        channel.done = true;
    }
    channel.cv.notify_all();
    pool.clear();

    std::print("{}\n", total.load(std::memory_order_relaxed));
    return 0;
}
