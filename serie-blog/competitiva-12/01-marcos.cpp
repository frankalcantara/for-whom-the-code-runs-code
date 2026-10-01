// T04.1, os marcos da estrada: lower_bound e os seus dois vizinhos.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> marco(n);                  // posições até 10^18, já em ordem crescente
    for (auto& m : marco) std::cin >> m;
    while (q-- > 0) {
        long long p = 0;
        std::cin >> p;
        const auto it = std::ranges::lower_bound(marco, p);   // primeiro marco >= p
        long long melhor = -1;
        if (it != marco.end()) melhor = *it - p;             // maior menos menor: nunca negativo
        if (it != marco.begin()) {
            const long long esquerda = p - *(it - 1);         // último marco < p
            if (melhor == -1 || esquerda < melhor) melhor = esquerda;
        }
        std::println("{}", melhor);
    }
}
