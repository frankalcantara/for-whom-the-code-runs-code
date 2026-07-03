#include <cstdlib>
#include <iostream>
#include <vector>

struct Vertex {
    long long x;
    long long y;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<Vertex> polygon(count);
    for (auto& [x, y] : polygon) {
        std::cin >> x >> y;
    }

    __int128 doubled_area = 0;
    for (int i = 0; i < count; ++i) {
        const Vertex& current = polygon[i];
        const Vertex& next = polygon[(i + 1) % count];
        doubled_area +=
            static_cast<__int128>(current.x) * next.y -
            static_cast<__int128>(next.x) * current.y;
    }
    if (doubled_area < 0) {
        doubled_area = -doubled_area;
    }

    if (doubled_area == 0) {
        std::cout << "0\n";
        return 0;
    }

    std::string digits;
    while (doubled_area > 0) {
        digits.push_back(
            static_cast<char>('0' + doubled_area % 10));
        doubled_area /= 10;
    }
    for (auto iterator = digits.rbegin(); iterator != digits.rend();
         ++iterator) {
        std::cout << *iterator;
    }
    std::cout << '\n';
}
