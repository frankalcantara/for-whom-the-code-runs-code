#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int animals = 0;
    std::cin >> animals;

    std::vector<int> identifier(static_cast<std::size_t>(animals));
    for (int& value : identifier) {
        std::cin >> value;
    }

    int inversions = 0;
    for (int left = 0; left < animals; ++left) {
        for (int right = left + 1; right < animals; ++right) {
            if (identifier[static_cast<std::size_t>(left)]
                > identifier[static_cast<std::size_t>(right)]) {
                ++inversions;
            }
        }
    }

    std::cout << inversions << '\n';
}
