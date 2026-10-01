// Exercício 4.1, os sensores adormecidos: contagem em intervalos fechados com lower_bound e upper_bound.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> pos(n);                   // coordenadas até 10^12 em valor absoluto
    for (auto& x : pos) std::cin >> x;
    std::ranges::sort(pos);                          // pago uma vez: O(n log n)
    while (q-- > 0) {
        long long a = 0, b = 0;
        std::cin >> a >> b;
        const auto primeiro = std::ranges::lower_bound(pos, a);   // primeira coordenada >= a
        const auto depois = std::ranges::upper_bound(pos, b);     // primeira coordenada > b
        std::println("{}", depois - primeiro);
    }
}
