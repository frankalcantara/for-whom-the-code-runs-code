#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int notes = 0;
    std::cin >> notes;

    std::vector<long long> tails;
    tails.reserve(static_cast<std::size_t>(notes));

    for (int i = 0; i < notes; ++i) {
        long long pitch = 0;
        std::cin >> pitch;
        auto position = std::ranges::lower_bound(tails, pitch);
        if (position == tails.end()) {
            tails.push_back(pitch);
        } else {
            *position = pitch;
        }
    }

    std::cout << tails.size() << '\n';
}
