#include <algorithm>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<long long> colors(count);
    for (long long& color : colors) std::cin >> color;

    constexpr long long modulus = 1'000'000'007LL;
    std::vector<long long> restored(count);
    std::transform(
        std::execution::par_unseq,
        colors.begin(), colors.end(), restored.begin(),
        [](long long color) {
            return (color * color + 3 * color + 7) % modulus;
        });

    const long long checksum = std::reduce(
        std::execution::par,
        restored.begin(), restored.end(), 0LL, std::plus<>{});
    std::cout << checksum << '\n';
}
