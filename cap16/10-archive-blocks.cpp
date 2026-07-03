#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    int task_count;
    std::cin >> count >> task_count;
    std::vector<long long> values(count);
    for (long long& value : values) std::cin >> value;

    task_count = std::clamp(task_count, 1, count);
    const int block_size = (count + task_count - 1) / task_count;
    std::vector<std::future<long long>> futures;

    for (int task = 0; task < task_count; ++task) {
        const int begin = task * block_size;
        const int end = std::min(begin + block_size, count);
        if (begin >= end) break;
        futures.push_back(std::async(std::launch::async, [&, begin, end] {
            return std::accumulate(
                values.begin() + begin, values.begin() + end, 0LL);
        }));
    }

    long long total = 0;
    for (auto& future : futures) total += future.get();
    std::cout << total << '\n';
}
