#include <bit>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t universe = 0;
    std::size_t register_count = 0;
    int query_count = 0;
    std::cin >> universe >> register_count >> query_count;

    const std::size_t words = (universe + 63) / 64;
    std::vector<std::uint64_t> bits(register_count * words, 0);

    for (std::size_t registration = 0; registration < register_count;
         ++registration) {
        int star_count = 0;
        std::cin >> star_count;

        const std::size_t row = registration * words;
        for (int i = 0; i < star_count; ++i) {
            std::size_t star = 0;
            std::cin >> star;
            --star;

            bits[row + star / 64] |=
                std::uint64_t{1} << (star % 64);
        }
    }

    for (int query = 0; query < query_count; ++query) {
        int type = 0;
        std::size_t first = 0;
        std::size_t second = 0;
        std::cin >> type >> first >> second;
        --first;
        --second;

        const std::size_t first_row = first * words;
        const std::size_t second_row = second * words;
        std::size_t answer = 0;

        for (std::size_t word = 0; word < words; ++word) {
            const std::uint64_t left = bits[first_row + word];
            const std::uint64_t right = bits[second_row + word];

            std::uint64_t result = 0;
            switch (type) {
            case 1:
                result = left & right;
                break;
            case 2:
                result = left | right;
                break;
            case 3:
                result = left ^ right;
                break;
            case 4:
                result = left & ~right;
                break;
            }

            answer += std::popcount(result);
        }

        std::cout << answer << '\n';
    }
}
