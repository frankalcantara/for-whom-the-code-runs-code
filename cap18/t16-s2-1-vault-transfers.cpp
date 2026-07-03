// Exercise T16.2 - The Vault Transfers
// Session: T16.S2, exercise 1 of 1 (1 hour)
// Theme: chapter 16 (mutexes and scoped_lock)
// Tags: chapter-16, parallelism
//
// MODEL: several account balances are updated by independent
//   transfer records. Each transfer changes two balances that form
//   one invariant: the total amount across all accounts is
//   preserved.
// MATH: transfer (u, v, x) applies balance[u] -= x and
//   balance[v] += x. Both writes must happen while both account
//   mutexes are held, so no other worker observes a half-transfer.
// PROOF: invariant argument. Each completed transfer preserves the
//   total balance because it subtracts and adds the same amount.
//   std::scoped_lock acquires both involved mutexes as one critical
//   section, so transfers touching the same account are serialized.
//   Transfers on disjoint accounts may commute freely.
// COMPLEXITY: O(m) total transfer work plus synchronization cost.
//   Memory is O(a + m).
// TYPES: balances can accumulate up to about 2e14 in magnitude, so
//   int64_t is used for balances and amounts.
// ALTERNATIVES: atomics on individual balances would not make the
//   two-balance transfer an atomic transaction; processing
//   sequentially is simpler but does not train the lock invariant.
// EDGE CASES: m = 0; p > m; negative initial balances; transfers
//   in both directions between the same pair.
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -pthread -o transfers t16-s2-1-vault-transfers.cpp

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

struct Account {
    std::int64_t balance = 0;
    std::mutex mutex;
};

struct Transfer {
    int from = 0;
    int to = 0;
    std::int64_t amount = 0;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int accounts_count = 0;
    int transfers_count = 0;
    int workers = 0;
    std::cin >> accounts_count >> transfers_count >> workers;

    std::vector<std::unique_ptr<Account>> accounts;
    accounts.reserve(static_cast<std::size_t>(accounts_count));
    for (int i = 0; i < accounts_count; ++i) {
        auto account = std::make_unique<Account>();
        std::cin >> account->balance;
        accounts.push_back(std::move(account));
    }

    std::vector<Transfer> transfers(transfers_count);
    for (Transfer& transfer : transfers) {
        std::cin >> transfer.from >> transfer.to >> transfer.amount;
        --transfer.from;
        --transfer.to;
    }

    workers = std::min(workers, std::max(1, transfers_count));
    std::vector<std::jthread> pool;
    pool.reserve(static_cast<std::size_t>(workers));
    for (int id = 0; id < workers; ++id) {
        pool.emplace_back([&, id] {
            for (int i = id; i < transfers_count; i += workers) {
                const Transfer& transfer = transfers[i];
                Account& from = *accounts[transfer.from];
                Account& to = *accounts[transfer.to];
                std::scoped_lock lock(from.mutex, to.mutex);
                from.balance -= transfer.amount;
                to.balance += transfer.amount;
            }
        });
    }
    pool.clear();

    for (int i = 0; i < accounts_count; ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << accounts[i]->balance;
    }
    std::cout << '\n';
    return 0;
}
