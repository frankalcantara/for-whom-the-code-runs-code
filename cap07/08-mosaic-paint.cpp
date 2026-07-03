#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int rows = 0;
    int columns = 0;
    int operations = 0;
    std::cin >> rows >> columns >> operations;

    std::vector<int> grid(
        static_cast<std::size_t>(rows * columns), 0);

    for (int operation = 0; operation < operations; ++operation) {
        int first_row = 0;
        int first_column = 0;
        int last_row = 0;
        int last_column = 0;
        int color = 0;
        std::cin >> first_row >> first_column >> last_row >> last_column
                 >> color;

        for (int row = first_row - 1; row < last_row; ++row) {
            for (int column = first_column - 1; column < last_column;
                 ++column) {
                grid[static_cast<std::size_t>(row * columns + column)] =
                    color;
            }
        }
    }

    std::vector<char> visible(static_cast<std::size_t>(operations + 1), false);
    for (int color : grid) {
        if (color != 0) {
            visible[static_cast<std::size_t>(color)] = true;
        }
    }

    int answer = 0;
    for (char color_is_visible : visible) {
        answer += color_is_visible;
    }
    std::cout << answer << '\n';
}
