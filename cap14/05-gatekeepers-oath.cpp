#include <iostream>
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

    std::vector<unsigned char> chosen(n);
    for (const auto [u, v] : edges) {
        if (!chosen[u] && !chosen[v]) {
            chosen[u] = 1;
            chosen[v] = 1;
        }
    }

    int count = 0;
    for (unsigned char value : chosen) count += value;
    std::cout << count << '\n';
    for (int vertex = 0; vertex < n; ++vertex) {
        if (chosen[vertex]) std::cout << vertex + 1 << ' ';
    }
    std::cout << '\n';
}
