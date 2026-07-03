#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int width = 0;
    std::cin >> n >> width;

    std::vector<long long> energy(static_cast<std::size_t>(n));
    for (long long& value : energy) {
        std::cin >> value;
    }

    long long window_sum = 0;
    for (int i = 0; i < width; ++i) {
        window_sum += energy[static_cast<std::size_t>(i)];
    }

    long long minimum_sum = window_sum;
    long long maximum_sum = window_sum;

    for (int right = width; right < n; ++right) {
        window_sum += energy[static_cast<std::size_t>(right)];
        window_sum -= energy[static_cast<std::size_t>(right - width)];
        minimum_sum = std::min(minimum_sum, window_sum);
        maximum_sum = std::max(maximum_sum, window_sum);
    }

    std::cout << minimum_sum << ' ' << maximum_sum << '\n';
}
