#include <array>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rows = 0;
    int columns = 0;
    std::cin >> rows >> columns;

    const int cells = rows * columns;
    std::vector<int> color(static_cast<std::size_t>(cells));
    for (int& value : color) {
        std::cin >> value;
    }

    std::vector<char> visited(static_cast<std::size_t>(cells), false);
    constexpr std::array<int, 4> delta_row{-1, 1, 0, 0};
    constexpr std::array<int, 4> delta_column{0, 0, -1, 1};
    int regions = 0;

    for (int start = 0; start < cells; ++start) {
        if (visited[static_cast<std::size_t>(start)]) {
            continue;
        }
        ++regions;
        visited[static_cast<std::size_t>(start)] = true;
        std::queue<int> pending;
        pending.push(start);

        while (!pending.empty()) {
            const int current = pending.front();
            pending.pop();
            const int row = current / columns;
            const int column = current % columns;

            for (int direction = 0; direction < 4; ++direction) {
                const int next_row =
                    row + delta_row[static_cast<std::size_t>(direction)];
                const int next_column =
                    column
                    + delta_column[static_cast<std::size_t>(direction)];
                if (next_row < 0 || next_row >= rows || next_column < 0
                    || next_column >= columns) {
                    continue;
                }
                const int next = next_row * columns + next_column;
                if (!visited[static_cast<std::size_t>(next)]
                    && color[static_cast<std::size_t>(next)]
                           == color[static_cast<std::size_t>(current)]) {
                    visited[static_cast<std::size_t>(next)] = true;
                    pending.push(next);
                }
            }
        }
    }

    std::cout << regions << '\n';
}
