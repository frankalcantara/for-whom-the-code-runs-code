#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

struct Canopy {
    int left;
    int right;
    long long cost;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int canopies = 0;
    int final_position = 0;
    std::cin >> canopies >> final_position;

    std::vector<Canopy> offer(static_cast<std::size_t>(canopies));
    for (Canopy& canopy : offer) {
        std::cin >> canopy.left >> canopy.right >> canopy.cost;
    }

    constexpr long long infinity =
        std::numeric_limits<long long>::max() / 4;
    std::vector<long long> best(
        static_cast<std::size_t>(final_position + 1), infinity);
    best[0] = 0;

    for (int covered = 0; covered < final_position; ++covered) {
        if (best[static_cast<std::size_t>(covered)] == infinity) {
            continue;
        }
        for (const Canopy& canopy : offer) {
            if (canopy.left <= covered + 1 && canopy.right > covered) {
                best[static_cast<std::size_t>(canopy.right)] =
                    std::min(
                        best[static_cast<std::size_t>(canopy.right)],
                        best[static_cast<std::size_t>(covered)] + canopy.cost);
            }
        }
    }

    const long long answer = best[static_cast<std::size_t>(final_position)];
    std::cout << (answer == infinity ? -1 : answer) << '\n';
}
