#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int stations = 0;
    std::uint64_t steps = 0;
    std::cin >> stations >> steps;

    std::vector<int> shift(static_cast<std::size_t>(stations));
    for (int& value : shift) {
        std::cin >> value;
    }

    std::vector<int> first_visit(static_cast<std::size_t>(stations), -1);
    std::vector<int> path;
    path.reserve(static_cast<std::size_t>(stations));

    int current = 0;
    while (first_visit[static_cast<std::size_t>(current)] == -1) {
        first_visit[static_cast<std::size_t>(current)] =
            static_cast<int>(path.size());
        path.push_back(current);
        current =
            (current + shift[static_cast<std::size_t>(current)]) % stations;
    }

    const std::uint64_t cycle_start = static_cast<std::uint64_t>(
        first_visit[static_cast<std::size_t>(current)]);
    const std::uint64_t cycle_length =
        static_cast<std::uint64_t>(path.size()) - cycle_start;

    std::uint64_t path_index = steps;
    if (path_index >= path.size()) {
        path_index =
            cycle_start + (path_index - cycle_start) % cycle_length;
    }

    std::cout << path[static_cast<std::size_t>(path_index)] + 1 << '\n';
}
