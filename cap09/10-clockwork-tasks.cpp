#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int tasks = 0;
    int dependencies = 0;
    std::cin >> tasks >> dependencies;

    std::vector<std::vector<int>> graph(static_cast<std::size_t>(tasks));
    std::vector<int> indegree(static_cast<std::size_t>(tasks), 0);
    for (int i = 0; i < dependencies; ++i) {
        int before = 0;
        int after = 0;
        std::cin >> before >> after;
        --before;
        --after;
        graph[static_cast<std::size_t>(before)].push_back(after);
        ++indegree[static_cast<std::size_t>(after)];
    }

    std::priority_queue<int, std::vector<int>, std::greater<>> ready;
    for (int task = 0; task < tasks; ++task) {
        if (indegree[static_cast<std::size_t>(task)] == 0) {
            ready.push(task);
        }
    }

    std::vector<int> order;
    order.reserve(static_cast<std::size_t>(tasks));
    while (!ready.empty()) {
        const int task = ready.top();
        ready.pop();
        order.push_back(task);
        for (int next : graph[static_cast<std::size_t>(task)]) {
            if (--indegree[static_cast<std::size_t>(next)] == 0) {
                ready.push(next);
            }
        }
    }

    if (static_cast<int>(order.size()) != tasks) {
        std::cout << "CYCLE\n";
        return 0;
    }

    for (int i = 0; i < tasks; ++i) {
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << order[static_cast<std::size_t>(i)] + 1;
    }
    std::cout << '\n';
}
