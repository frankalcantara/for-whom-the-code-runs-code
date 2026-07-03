#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

struct Channel {
    std::deque<long long> values;
    std::size_t capacity;
    bool closed = false;
    std::mutex mutex;
    std::condition_variable not_empty;
    std::condition_variable not_full;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::size_t capacity;
    std::cin >> count >> capacity;
    std::vector<long long> input(count);
    for (long long& value : input) std::cin >> value;

    Channel channel;
    channel.capacity = capacity;
    long long sum = 0;

    std::jthread producer([&] {
        for (long long value : input) {
            std::unique_lock lock{channel.mutex};
            channel.not_full.wait(lock, [&] {
                return channel.values.size() < channel.capacity;
            });
            channel.values.push_back(value);
            lock.unlock();
            channel.not_empty.notify_one();
        }
        {
            std::lock_guard lock{channel.mutex};
            channel.closed = true;
        }
        channel.not_empty.notify_all();
    });

    std::jthread consumer([&] {
        while (true) {
            std::unique_lock lock{channel.mutex};
            channel.not_empty.wait(lock, [&] {
                return !channel.values.empty() || channel.closed;
            });
            if (channel.values.empty() && channel.closed) break;
            const long long value = channel.values.front();
            channel.values.pop_front();
            lock.unlock();
            channel.not_full.notify_one();
            sum += value;
        }
    });

    producer.join();
    consumer.join();
    std::cout << sum << '\n';
}
