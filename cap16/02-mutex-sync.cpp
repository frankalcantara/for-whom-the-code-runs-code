// cap16/02-mutex-sync.cpp
// Mutexes, std::scoped_lock, and std::condition_variable.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -pthread -o mutex_sync 02-mutex-sync.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Femutex_sync.exe 02-mutex-sync.cpp

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <print>
#include <queue>
#include <thread>
#include <vector>

struct BankAccount {
    long long balance{0};
    std::mutex mtx;
};

void transfer(BankAccount& src, BankAccount& dst, long long amount) {
    std::scoped_lock lk{src.mtx, dst.mtx};
    src.balance -= amount;
    dst.balance += amount;
}

void scoped_lock_example() {
    BankAccount a{1000}, b{1000};
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([&, i] {
            for (int j = 0; j < 10'000; ++j) {
                if (i % 2 == 0) transfer(a, b, 1);
                else            transfer(b, a, 1);
            }
        });
    }

    for (auto& t : threads) t.join();

    std::scoped_lock lk{a.mtx, b.mtx};
    assert(a.balance + b.balance == 2000);
}

struct Channel {
    std::queue<int> q;
    std::mutex mtx;
    std::condition_variable cv;
    bool done{false};
};

void producer(Channel& ch, int n) {
    for (int i = 0; i < n; ++i) {
        {
            std::lock_guard lk{ch.mtx};
            ch.q.push(i);
        }
        ch.cv.notify_one();
    }
    {
        std::lock_guard lk{ch.mtx};
        ch.done = true;
    }
    ch.cv.notify_all();
}

long long consumer(Channel& ch) {
    long long sum = 0;
    while (true) {
        std::unique_lock lk{ch.mtx};
        ch.cv.wait(lk, [&] { return !ch.q.empty() || ch.done; });
        while (!ch.q.empty()) {
            sum += ch.q.front();
            ch.q.pop();
        }
        if (ch.done) break;
    }
    return sum;
}

void condition_variable_example(int n) {
    Channel ch;
    long long result = 0;

    std::thread prod{producer, std::ref(ch), n};
    std::thread cons{[&] { result = consumer(ch); }};
    prod.join();
    cons.join();

    long long expected = 1LL * n * (n - 1) / 2;
    assert(result == expected);
}

int main() {
    scoped_lock_example();
    condition_variable_example(100'000);
    std::print("02-mutex-sync: all assertions passed\n");
    return 0;
}
