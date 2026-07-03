#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace {

int gesture_index(char gesture) {
    if (gesture == 'R') {
        return 0;
    }
    if (gesture == 'P') {
        return 1;
    }
    return 2;
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    std::string gestures;
    std::cin >> gestures;

    std::vector<std::array<int, 3>> prefix(
        static_cast<std::size_t>(n + 1), {0, 0, 0});

    for (int i = 0; i < n; ++i) {
        prefix[static_cast<std::size_t>(i + 1)] =
            prefix[static_cast<std::size_t>(i)];
        ++prefix[static_cast<std::size_t>(i + 1)]
                [static_cast<std::size_t>(gesture_index(
                    gestures[static_cast<std::size_t>(i)]))];
    }

    while (q-- > 0) {
        int left = 0;
        int right = 0;
        std::cin >> left >> right;

        for (int gesture = 0; gesture < 3; ++gesture) {
            if (gesture != 0) {
                std::cout << ' ';
            }
            std::cout
                << prefix[static_cast<std::size_t>(right)]
                         [static_cast<std::size_t>(gesture)]
                 - prefix[static_cast<std::size_t>(left - 1)]
                         [static_cast<std::size_t>(gesture)];
        }
        std::cout << '\n';
    }
}
