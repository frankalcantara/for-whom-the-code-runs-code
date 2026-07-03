#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

struct Lantern {
    long long x;
    long long y;
};

using Distance = unsigned long long;

Distance squared_distance(const Lantern& first, const Lantern& second) {
    const __int128 dx = static_cast<__int128>(first.x) - second.x;
    const __int128 dy = static_cast<__int128>(first.y) - second.y;
    return static_cast<Distance>(dx * dx + dy * dy);
}

Distance closest(std::vector<Lantern>& points, std::vector<Lantern>& buffer,
                 int left, int right) {
    if (right - left <= 3) {
        Distance best = std::numeric_limits<Distance>::max();
        for (int i = left; i < right; ++i) {
            for (int j = i + 1; j < right; ++j) {
                best = std::min(best, squared_distance(points[i], points[j]));
            }
        }
        std::ranges::sort(points.begin() + left, points.begin() + right,
                          {}, &Lantern::y);
        return best;
    }

    const int middle = (left + right) / 2;
    const long long middle_x = points[middle].x;
    Distance best = std::min(
        closest(points, buffer, left, middle),
        closest(points, buffer, middle, right));

    std::ranges::merge(points.begin() + left, points.begin() + middle,
                       points.begin() + middle, points.begin() + right,
                       buffer.begin(), {}, &Lantern::y, &Lantern::y);
    std::copy_n(buffer.begin(), right - left, points.begin() + left);

    std::vector<Lantern> strip;
    strip.reserve(right - left);
    for (int i = left; i < right; ++i) {
        const __int128 dx = static_cast<__int128>(points[i].x) - middle_x;
        if (dx * dx < best) {
            strip.push_back(points[i]);
        }
    }

    for (int i = 0; i < static_cast<int>(strip.size()); ++i) {
        for (int j = i + 1; j < static_cast<int>(strip.size()); ++j) {
            const __int128 dy =
                static_cast<__int128>(strip[j].y) - strip[i].y;
            if (dy * dy >= best) {
                break;
            }
            best = std::min(best, squared_distance(strip[i], strip[j]));
        }
    }
    return best;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<Lantern> points(count);
    for (auto& [x, y] : points) {
        std::cin >> x >> y;
    }
    std::ranges::sort(points, [](const Lantern& first, const Lantern& second) {
        return first.x != second.x ? first.x < second.x : first.y < second.y;
    });

    std::vector<Lantern> buffer(count);
    std::cout << closest(points, buffer, 0, count) << '\n';
}
