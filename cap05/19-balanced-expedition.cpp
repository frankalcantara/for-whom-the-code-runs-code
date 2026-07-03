#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    long long tolerance = 0;
    std::cin >> n >> tolerance;

    std::vector<long long> skill(static_cast<std::size_t>(n));
    for (long long& value : skill) {
        std::cin >> value;
    }

    std::ranges::sort(skill);

    int best = 0;
    int left = 0;
    for (int right = 0; right < n; ++right) {
        while (skill[static_cast<std::size_t>(right)]
                   - skill[static_cast<std::size_t>(left)]
               > tolerance) {
            ++left;
        }
        best = std::max(best, right - left + 1);
    }

    std::cout << best << '\n';
}
