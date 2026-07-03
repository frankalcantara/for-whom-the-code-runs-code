#include <algorithm>
#include <cstdlib>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int commands = 0;
    std::cin >> commands;

    long long position = 0;
    long long farthest = 0;

    while (commands-- > 0) {
        char direction = '\0';
        long long distance = 0;
        std::cin >> direction >> distance;

        position += direction == 'L' ? -distance : distance;
        farthest = std::max(farthest, std::abs(position));
    }

    std::cout << position << ' ' << farthest << '\n';
}
