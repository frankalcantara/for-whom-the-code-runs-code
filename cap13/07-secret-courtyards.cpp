#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int query_count;
    std::cin >> query_count;
    while (query_count-- > 0) {
        long long x1;
        long long y1;
        long long x2;
        long long y2;
        long long px;
        long long py;
        std::cin >> x1 >> y1 >> x2 >> y2 >> px >> py;

        const auto [left, right] = std::minmax(x1, x2);
        const auto [bottom, top] = std::minmax(y1, y2);
        const bool inside =
            left < px && px < right && bottom < py && py < top;
        std::cout << (inside ? "YES" : "NO") << '\n';
    }
}
