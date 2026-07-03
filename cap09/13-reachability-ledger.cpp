#include <bit>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertices = 0;
    int edges = 0;
    std::cin >> vertices >> edges;

    const std::size_t words =
        (static_cast<std::size_t>(vertices) + 63) / 64;
    std::vector<std::uint64_t> reachable(
        static_cast<std::size_t>(vertices) * words, 0);

    for (int vertex = 0; vertex < vertices; ++vertex) {
        reachable[static_cast<std::size_t>(vertex) * words
                  + static_cast<std::size_t>(vertex / 64)]
            |= std::uint64_t{1} << (vertex % 64);
    }
    for (int i = 0; i < edges; ++i) {
        int from = 0;
        int to = 0;
        std::cin >> from >> to;
        --from;
        --to;
        reachable[static_cast<std::size_t>(from) * words
                  + static_cast<std::size_t>(to / 64)]
            |= std::uint64_t{1} << (to % 64);
    }

    for (int middle = 0; middle < vertices; ++middle) {
        const std::size_t middle_word =
            static_cast<std::size_t>(middle / 64);
        const std::uint64_t middle_bit =
            std::uint64_t{1} << (middle % 64);
        const std::size_t through_row =
            static_cast<std::size_t>(middle) * words;

        for (int from = 0; from < vertices; ++from) {
            const std::size_t row =
                static_cast<std::size_t>(from) * words;
            if ((reachable[row + middle_word] & middle_bit) == 0) {
                continue;
            }
            for (std::size_t word = 0; word < words; ++word) {
                reachable[row + word] |= reachable[through_row + word];
            }
        }
    }

    long long answer = -vertices;
    for (std::uint64_t word : reachable) {
        answer += std::popcount(word);
    }
    std::cout << answer << '\n';
}
