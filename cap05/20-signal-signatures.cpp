#include <array>
#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;

    std::array<long long, 65> count_by_width{};
    std::array<long long, 65> bits_by_width{};

    for (int i = 0; i < n; ++i) {
        std::uint64_t signal = 0;
        std::cin >> signal;

        const unsigned int width = std::bit_width(signal);
        ++count_by_width[width];
        bits_by_width[width] += std::popcount(signal);
    }

    for (std::size_t width = 0; width < count_by_width.size(); ++width) {
        if (count_by_width[width] != 0) {
            std::cout << width << ' ' << count_by_width[width] << ' '
                      << bits_by_width[width] << '\n';
        }
    }
}
