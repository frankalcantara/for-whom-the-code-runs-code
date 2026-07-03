#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using Edge = std::pair<int, long long>;

std::pair<int, long long> farthest_from(
    int source,
    const std::vector<std::vector<Edge>>& graph) {
    std::vector<long long> distance(graph.size(), -1);
    std::queue<int> pending;
    distance[static_cast<std::size_t>(source)] = 0;
    pending.push(source);

    int farthest = source;
    while (!pending.empty()) {
        const int vertex = pending.front();
        pending.pop();
        for (const auto [next, weight] :
             graph[static_cast<std::size_t>(vertex)]) {
            if (distance[static_cast<std::size_t>(next)] != -1) {
                continue;
            }
            distance[static_cast<std::size_t>(next)] =
                distance[static_cast<std::size_t>(vertex)] + weight;
            if (distance[static_cast<std::size_t>(next)]
                > distance[static_cast<std::size_t>(farthest)]) {
                farthest = next;
            }
            pending.push(next);
        }
    }
    return {farthest, distance[static_cast<std::size_t>(farthest)]};
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertices = 0;
    std::cin >> vertices;

    std::vector<std::vector<Edge>> graph(
        static_cast<std::size_t>(vertices));
    for (int i = 1; i < vertices; ++i) {
        int a = 0;
        int b = 0;
        long long length = 0;
        std::cin >> a >> b >> length;
        --a;
        --b;
        graph[static_cast<std::size_t>(a)].push_back({b, length});
        graph[static_cast<std::size_t>(b)].push_back({a, length});
    }

    const auto [endpoint, ignored] = farthest_from(0, graph);
    const auto [other_endpoint, diameter] =
        farthest_from(endpoint, graph);
    static_cast<void>(ignored);
    static_cast<void>(other_endpoint);
    std::cout << diameter << '\n';
}
