// Exercise 2.4 - The Caravan Ledger
// Aggregate cargo and identify the caravan carrying the greatest load.
// Time: O(n). Space: O(n) for the records.

#include <iostream>
#include <print>
#include <string>
#include <vector>

struct Caravan {
    std::string name;
    long long crates;
    long long weight_per_crate;
};

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<Caravan> caravans(n);
    for (auto& [name, crates, weight] : caravans)
        std::cin >> name >> crates >> weight;

    long long total_crates = 0;
    long long total_weight = 0;
    std::string heaviest_name;
    long long heaviest_load = -1;

    for (const auto& [name, crates, weight] : caravans) {
        const long long load = crates * weight;
        total_crates += crates;
        total_weight += load;

        if (load > heaviest_load ||
            (load == heaviest_load && name < heaviest_name)) {
            heaviest_load = load;
            heaviest_name = name;
        }
    }

    std::print("{} {}\n", total_crates, total_weight);
    std::print("{} {}\n", heaviest_name, heaviest_load);
}
