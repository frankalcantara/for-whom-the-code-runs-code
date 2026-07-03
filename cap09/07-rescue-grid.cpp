#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rows = 0;
    int columns = 0;
    std::cin >> rows >> columns;

    std::vector<std::string> grid(static_cast<std::size_t>(rows));
    for (std::string& row : grid) {
        std::cin >> row;
    }

    int source_row = 0;
    int source_column = 0;
    int target_row = 0;
    int target_column = 0;
    std::cin >> source_row >> source_column >> target_row >> target_column;
    --source_row;
    --source_column;
    --target_row;
    --target_column;

    const auto id = [columns](int row, int column) {
        return row * columns + column;
    };

    std::vector<int> distance(
        static_cast<std::size_t>(rows * columns), -1);
    std::queue<int> pending;
    distance[static_cast<std::size_t>(id(source_row, source_column))] = 0;
    pending.push(id(source_row, source_column));

    constexpr std::array<int, 4> delta_row{-1, 1, 0, 0};
    constexpr std::array<int, 4> delta_column{0, 0, -1, 1};

    while (!pending.empty()) {
        const int current = pending.front();
        pending.pop();
        const int row = current / columns;
        const int column = current % columns;

        for (int direction = 0; direction < 4; ++direction) {
            const int next_row =
                row + delta_row[static_cast<std::size_t>(direction)];
            const int next_column =
                column + delta_column[static_cast<std::size_t>(direction)];
            if (next_row < 0 || next_row >= rows || next_column < 0
                || next_column >= columns
                || grid[static_cast<std::size_t>(next_row)]
                       [static_cast<std::size_t>(next_column)] == '#') {
                continue;
            }
            const int next = id(next_row, next_column);
            if (distance[static_cast<std::size_t>(next)] != -1) {
                continue;
            }
            distance[static_cast<std::size_t>(next)] =
                distance[static_cast<std::size_t>(current)] + 1;
            pending.push(next);
        }
    }

    std::cout
        << distance[static_cast<std::size_t>(
               id(target_row, target_column))]
        << '\n';
}
