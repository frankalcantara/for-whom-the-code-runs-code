#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<long long> tide(count);
    for (long long& value : tide) std::cin >> value;

    long long total = tide[0];
    long long best_ending = tide[0];
    long long best = tide[0];
    long long worst_ending = tide[0];
    long long worst = tide[0];

    for (int i = 1; i < count; ++i) {
        total += tide[i];
        best_ending = std::max(tide[i], best_ending + tide[i]);
        best = std::max(best, best_ending);
        worst_ending = std::min(tide[i], worst_ending + tide[i]);
        worst = std::min(worst, worst_ending);
    }

    const long long answer =
        (worst == total) ? best : std::max(best, total - worst);
    std::cout << answer << '\n';
}
