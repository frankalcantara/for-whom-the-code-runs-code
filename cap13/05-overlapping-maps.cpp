#include <algorithm>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long ax1;
    long long ay1;
    long long ax2;
    long long ay2;
    long long bx1;
    long long by1;
    long long bx2;
    long long by2;
    std::cin >> ax1 >> ay1 >> ax2 >> ay2;
    std::cin >> bx1 >> by1 >> bx2 >> by2;

    const long long width =
        std::max(0LL, std::min(ax2, bx2) - std::max(ax1, bx1));
    const long long height =
        std::max(0LL, std::min(ay2, by2) - std::max(ay1, by1));

    std::cout << width * height << '\n';
}
