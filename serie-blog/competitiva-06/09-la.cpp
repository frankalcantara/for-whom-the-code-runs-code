// A1.2, o mercado de lã: ordenar uma vez e responder cada limite com upper_bound.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> peso(n);                  // pesos até 10^12
    for (auto& x : peso) std::cin >> x;
    std::ranges::sort(peso);
    while (q-- > 0) {
        long long limite = 0;
        std::cin >> limite;
        const auto fronteira = std::ranges::upper_bound(peso, limite);   // primeiro peso > limite
        std::println("{}", fronteira - peso.begin());
    }
}
