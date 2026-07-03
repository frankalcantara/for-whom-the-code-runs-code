#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int m;
    int colors;
    std::cin >> n >> m >> colors;
    std::vector<std::vector<int>> graph(n);
    while (m-- > 0) {
        int u;
        int v;
        std::cin >> u >> v;
        --u;
        --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::ranges::sort(order, [&](int first, int second) {
        return graph[first].size() > graph[second].size();
    });
    std::vector<int> color(n, -1);

    auto search = [&](auto&& self, int position) -> bool {
        if (position == n) return true;
        const int vertex = order[position];
        for (int candidate = 0; candidate < colors; ++candidate) {
            bool allowed = true;
            for (int neighbor : graph[vertex]) {
                if (color[neighbor] == candidate) {
                    allowed = false;
                    break;
                }
            }
            if (!allowed) continue;
            color[vertex] = candidate;
            if (self(self, position + 1)) return true;
            color[vertex] = -1;
        }
        return false;
    };

    if (!search(search, 0)) {
        std::cout << "NO\n";
        return 0;
    }
    std::cout << "YES\n";
    for (int value : color) std::cout << value + 1 << ' ';
    std::cout << '\n';
}
