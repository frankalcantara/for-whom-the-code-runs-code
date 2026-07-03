#include <algorithm>
#include <cmath>
#include <compare>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
    long long x;
    long long y;

    auto operator<=>(const Point&) const = default;
};

__int128 cross(const Point& origin, const Point& first,
               const Point& second) {
    return static_cast<__int128>(first.x - origin.x) *
               (second.y - origin.y) -
           static_cast<__int128>(first.y - origin.y) *
               (second.x - origin.x);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<Point> points(count);
    for (auto& [x, y] : points) {
        std::cin >> x >> y;
    }

    std::ranges::sort(points);
    points.erase(std::unique(points.begin(), points.end()), points.end());

    if (points.size() == 1) {
        std::cout << "0.00\n";
        return 0;
    }

    std::vector<Point> hull;
    for (const Point point : points) {
        while (hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull.back(), point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    const std::size_t lower_size = hull.size();
    for (auto iterator = points.rbegin() + 1; iterator != points.rend();
         ++iterator) {
        while (hull.size() > lower_size &&
               cross(hull[hull.size() - 2], hull.back(), *iterator) <= 0) {
            hull.pop_back();
        }
        hull.push_back(*iterator);
    }
    hull.pop_back();

    long double perimeter = 0;
    for (std::size_t i = 0; i < hull.size(); ++i) {
        const Point& first = hull[i];
        const Point& second = hull[(i + 1) % hull.size()];
        perimeter += std::hypot(
            static_cast<long double>(first.x - second.x),
            static_cast<long double>(first.y - second.y));
    }

    std::cout << std::fixed << std::setprecision(2) << perimeter << '\n';
}
