#include <bit>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int m;
    std::cin >> n >> m;
    std::vector<std::pair<int, int>> edges(m);
    for (auto& [u, v] : edges) {
        std::cin >> u >> v;
        --u;
        --v;
    }

    unsigned best_mask = (1U << n) - 1U;
    int best_size = n;
    for (unsigned mask = 0; mask < (1U << n); ++mask) {
        const int size = std::popcount(mask);
        if (size >= best_size) continue;
        bool valid = true;
        for (const auto [u, v] : edges) {
            if (!(mask & (1U << u)) && !(mask & (1U << v))) {
                valid = false;
                break;
            }
        }
        if (valid) {
            best_size = size;
            best_mask = mask;
        }
    }

    std::cout << best_size << '\n';
    for (int vertex = 0; vertex < n; ++vertex) {
        if (best_mask & (1U << vertex)) std::cout << vertex + 1 << ' ';
    }
    std::cout << '\n';
}
