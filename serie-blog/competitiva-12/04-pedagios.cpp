// T04.4, o quadro de pedágios: a k-ésima menor tarifa com nth_element, sem ordenar o resto.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<long long> tarifa(n);                 // tarifas até 10^18
    for (auto& t : tarifa) std::cin >> t;
    std::ranges::nth_element(tarifa, tarifa.begin() + (k - 1));
    std::println("{}", tarifa[k - 1]);
}
