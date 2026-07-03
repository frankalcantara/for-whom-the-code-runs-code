#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int workers;
    long long observations_per_worker;
    std::cin >> workers >> observations_per_worker;

    std::atomic<long long> observations{0};
    std::vector<std::jthread> threads;
    threads.reserve(workers);

    for (int worker = 0; worker < workers; ++worker) {
        threads.emplace_back([&] {
            for (long long i = 0; i < observations_per_worker; ++i) {
                observations.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    threads.clear();

    std::cout << observations.load(std::memory_order_relaxed) << '\n';
}
