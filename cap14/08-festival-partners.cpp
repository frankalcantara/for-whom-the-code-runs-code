#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int left_size;
    int right_size;
    int edge_count;
    std::cin >> left_size >> right_size >> edge_count;
    std::vector<std::vector<int>> graph(left_size);
    while (edge_count-- > 0) {
        int left;
        int right;
        std::cin >> left >> right;
        graph[left - 1].push_back(right - 1);
    }

    std::vector<int> matched_right(right_size, -1);
    auto augment = [&](auto&& self, int left,
                       std::vector<unsigned char>& seen) -> bool {
        for (int right : graph[left]) {
            if (seen[right]) continue;
            seen[right] = 1;
            if (matched_right[right] == -1 ||
                self(self, matched_right[right], seen)) {
                matched_right[right] = left;
                return true;
            }
        }
        return false;
    };

    int matching = 0;
    for (int left = 0; left < left_size; ++left) {
        std::vector<unsigned char> seen(right_size);
        matching += augment(augment, left, seen);
    }
    std::cout << left_size + right_size - matching << '\n';
}
