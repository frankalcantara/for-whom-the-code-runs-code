#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int cities = 0;
    int roads = 0;
    std::cin >> cities >> roads;

    using Edge = std::pair<int, long long>;
    std::vector<std::vector<Edge>> graph(
        static_cast<std::size_t>(cities));
    for (int i = 0; i < roads; ++i) {
        int from = 0;
        int to = 0;
        long long time = 0;
        std::cin >> from >> to >> time;
        graph[static_cast<std::size_t>(from - 1)].push_back({to - 1, time});
    }

    constexpr long long infinity =
        std::numeric_limits<long long>::max() / 4;
    std::vector<long long> distance(
        static_cast<std::size_t>(cities), infinity);
    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<>> pending;
    distance[0] = 0;
    pending.push({0, 0});

    while (!pending.empty()) {
        const auto [known_distance, city] = pending.top();
        pending.pop();
        if (known_distance != distance[static_cast<std::size_t>(city)]) {
            continue;
        }
        for (const auto [next, time] :
             graph[static_cast<std::size_t>(city)]) {
            const long long candidate = known_distance + time;
            if (candidate < distance[static_cast<std::size_t>(next)]) {
                distance[static_cast<std::size_t>(next)] = candidate;
                pending.push({candidate, next});
            }
        }
    }

    for (int city = 0; city < cities; ++city) {
        if (city != 0) {
            std::cout << ' ';
        }
        const long long value = distance[static_cast<std::size_t>(city)];
        std::cout << (value == infinity ? -1 : value);
    }
    std::cout << '\n';
}
