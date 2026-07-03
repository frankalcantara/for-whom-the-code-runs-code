#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int gears = 0;
    int selected = 0;
    std::cin >> gears >> selected;

    std::vector<std::uint64_t> tooth_count(
        static_cast<std::size_t>(gears));
    for (std::uint64_t& value : tooth_count) {
        std::cin >> value;
    }

    std::uint64_t best = std::numeric_limits<std::uint64_t>::max();
    const unsigned int all_masks = 1U << gears;

    for (unsigned int mask = 0; mask < all_masks; ++mask) {
        if (std::popcount(mask) != selected) {
            continue;
        }
        std::uint64_t current = 1;
        bool exceeded = false;
        for (int index = 0; index < gears; ++index) {
            if ((mask & (1U << index)) == 0) {
                continue;
            }
            const std::uint64_t value =
                tooth_count[static_cast<std::size_t>(index)];
            const std::uint64_t reduced = value / std::gcd(current, value);
            if (reduced > best / current) {
                exceeded = true;
                break;
            }
            current *= reduced;
        }
        if (!exceeded && current < best) {
            best = current;
        }
    }

    std::cout << best << '\n';
}
