#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

struct Rectangle {
    long long x1;
    long long y1;
    long long x2;
    long long y2;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long wall_width;
    long long wall_height;
    int count;
    std::cin >> wall_width >> wall_height >> count;

    std::vector<Rectangle> rectangles(count);
    std::vector<long long> xs{0, wall_width};
    std::vector<long long> ys{0, wall_height};
    for (auto& rectangle : rectangles) {
        std::cin >> rectangle.x1 >> rectangle.y1
                 >> rectangle.x2 >> rectangle.y2;
        xs.push_back(rectangle.x1);
        xs.push_back(rectangle.x2);
        ys.push_back(rectangle.y1);
        ys.push_back(rectangle.y2);
    }

    std::ranges::sort(xs);
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    std::ranges::sort(ys);
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

    const int width = static_cast<int>(xs.size());
    const int height = static_cast<int>(ys.size());
    std::vector<std::vector<int>> difference(
        width + 1, std::vector<int>(height + 1));

    for (const auto& rectangle : rectangles) {
        const int left =
            std::ranges::lower_bound(xs, rectangle.x1) - xs.begin();
        const int right =
            std::ranges::lower_bound(xs, rectangle.x2) - xs.begin();
        const int bottom =
            std::ranges::lower_bound(ys, rectangle.y1) - ys.begin();
        const int top =
            std::ranges::lower_bound(ys, rectangle.y2) - ys.begin();

        ++difference[left][bottom];
        --difference[right][bottom];
        --difference[left][top];
        ++difference[right][top];
    }

    long long covered_area = 0;
    for (int x = 0; x + 1 < width; ++x) {
        for (int y = 0; y + 1 < height; ++y) {
            if (x > 0) {
                difference[x][y] += difference[x - 1][y];
            }
            if (y > 0) {
                difference[x][y] += difference[x][y - 1];
            }
            if (x > 0 && y > 0) {
                difference[x][y] -= difference[x - 1][y - 1];
            }
            if (difference[x][y] > 0) {
                covered_area +=
                    (xs[x + 1] - xs[x]) * (ys[y + 1] - ys[y]);
            }
        }
    }

    std::cout << covered_area << '\n';
}
