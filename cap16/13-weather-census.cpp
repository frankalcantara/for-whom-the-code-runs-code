#include <algorithm>
#include <array>
#include <iostream>
#include <thread>
#include <vector>

struct alignas(64) LocalHistogram {
    std::array<unsigned long long, 256> count{};
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int sample_count;
    int worker_count;
    std::cin >> sample_count >> worker_count;
    std::vector<unsigned char> samples(sample_count);
    for (unsigned char& sample : samples) {
        int value;
        std::cin >> value;
        sample = static_cast<unsigned char>(value);
    }

    worker_count = std::clamp(worker_count, 1, sample_count);
    const int block_size = (sample_count + worker_count - 1) / worker_count;
    std::vector<LocalHistogram> local(worker_count);
    std::vector<std::jthread> workers;
    workers.reserve(worker_count);

    for (int worker = 0; worker < worker_count; ++worker) {
        const int begin = worker * block_size;
        const int end = std::min(begin + block_size, sample_count);
        workers.emplace_back([&, worker, begin, end] {
            for (int i = begin; i < end; ++i) {
                ++local[worker].count[samples[i]];
            }
        });
    }
    workers.clear();

    std::array<unsigned long long, 256> histogram{};
    for (const LocalHistogram& partial : local) {
        for (int value = 0; value < 256; ++value) {
            histogram[value] += partial.count[value];
        }
    }

    for (int value = 0; value < 256; ++value) {
        if (histogram[value] > 0) {
            std::cout << value << ' ' << histogram[value] << '\n';
        }
    }
}
