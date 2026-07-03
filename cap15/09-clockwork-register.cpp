#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int start;
    int target;
    std::cin >> start >> target;

    const int limit = std::max(start, target) * 2 + 2;
    std::vector<int> distance(limit + 1, -1);
    std::queue<int> pending;
    distance[start] = 0;
    pending.push(start);

    while (!pending.empty()) {
        const int value = pending.front();
        pending.pop();
        if (value == target) break;
        for (const int next : {value - 1, value + 1, value * 2}) {
            if (next < 0 || next > limit || distance[next] != -1) continue;
            distance[next] = distance[value] + 1;
            pending.push(next);
        }
    }

    std::cout << distance[target] << '\n';
}
