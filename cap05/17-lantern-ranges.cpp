#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int operations = 0;
    std::cin >> n >> operations;

    std::vector<long long> difference(static_cast<std::size_t>(n + 1), 0);

    while (operations-- > 0) {
        int left = 0;
        int right = 0;
        long long change = 0;
        std::cin >> left >> right >> change;

        difference[static_cast<std::size_t>(left - 1)] += change;
        difference[static_cast<std::size_t>(right)] -= change;
    }

    long long brightness = 0;
    for (int i = 0; i < n; ++i) {
        brightness += difference[static_cast<std::size_t>(i)];
        if (i != 0) {
            std::cout << ' ';
        }
        std::cout << brightness;
    }
    std::cout << '\n';
}
