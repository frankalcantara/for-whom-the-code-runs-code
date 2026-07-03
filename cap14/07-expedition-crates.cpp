#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

struct Item {
    int weight;
    int value;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int capacity;
    std::cin >> n >> capacity;
    std::vector<Item> items(n);
    for (auto& [weight, value] : items) std::cin >> weight >> value;
    std::ranges::sort(items, [](const Item& a, const Item& b) {
        return 1LL * a.value * b.weight > 1LL * b.value * a.weight;
    });

    long long best = 0;
    auto bound = [&](int position, int weight, long long value) {
        long long result = value;
        int remaining = capacity - weight;
        for (int i = position; i < n && remaining > 0; ++i) {
            if (items[i].weight <= remaining) {
                result += items[i].value;
                remaining -= items[i].weight;
            } else {
                const __int128 numerator =
                    static_cast<__int128>(remaining) * items[i].value;
                result += static_cast<long long>(
                    (numerator + items[i].weight - 1) / items[i].weight);
                break;
            }
        }
        return result;
    };

    auto search = [&](auto&& self, int position, int weight,
                      long long value) -> void {
        if (weight > capacity) return;
        best = std::max(best, value);
        if (position == n || bound(position, weight, value) <= best) return;
        self(self, position + 1, weight + items[position].weight,
             value + items[position].value);
        self(self, position + 1, weight, value);
    };
    search(search, 0, 0, 0);
    std::cout << best << '\n';
}
