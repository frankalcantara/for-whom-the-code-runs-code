#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int junctions = 0;
    int pipes = 0;
    int questions = 0;
    std::cin >> junctions >> pipes >> questions;

    std::vector<long long> strongest(
        static_cast<std::size_t>(junctions), 0);
    for (int i = 0; i < pipes; ++i) {
        int a = 0;
        int b = 0;
        long long capacity = 0;
        std::cin >> a >> b >> capacity;
        --a;
        --b;
        strongest[static_cast<std::size_t>(a)] =
            std::max(strongest[static_cast<std::size_t>(a)], capacity);
        strongest[static_cast<std::size_t>(b)] =
            std::max(strongest[static_cast<std::size_t>(b)], capacity);
    }

    std::ranges::sort(strongest);
    while (questions-- > 0) {
        long long threshold = 0;
        std::cin >> threshold;
        const auto first_operational =
            std::ranges::lower_bound(strongest, threshold);
        std::cout << std::distance(strongest.begin(), first_operational)
                  << '\n';
    }
}
