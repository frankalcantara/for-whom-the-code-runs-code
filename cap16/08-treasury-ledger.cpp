#include <atomic>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

struct Account {
    long long balance = 0;
    std::mutex mutex;
};

struct Transfer {
    int source;
    int destination;
    long long amount;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int account_count;
    int transfer_count;
    int worker_count;
    std::cin >> account_count >> transfer_count >> worker_count;

    std::deque<Account> accounts(account_count);
    for (Account& account : accounts) std::cin >> account.balance;

    std::vector<Transfer> transfers(transfer_count);
    for (auto& [source, destination, amount] : transfers) {
        std::cin >> source >> destination >> amount;
        --source;
        --destination;
    }

    std::atomic<int> next_transfer{0};
    std::vector<std::jthread> workers;
    workers.reserve(worker_count);
    for (int worker = 0; worker < worker_count; ++worker) {
        workers.emplace_back([&] {
            while (true) {
                const int index =
                    next_transfer.fetch_add(1, std::memory_order_relaxed);
                if (index >= transfer_count) break;

                const auto [source, destination, amount] = transfers[index];
                std::scoped_lock lock{
                    accounts[source].mutex, accounts[destination].mutex};
                accounts[source].balance -= amount;
                accounts[destination].balance += amount;
            }
        });
    }
    workers.clear();

    for (const Account& account : accounts) {
        std::cout << account.balance << ' ';
    }
    std::cout << '\n';
}
