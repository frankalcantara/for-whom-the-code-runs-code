// Exercício 4.4, o erro do copista: a repetição fica vizinha depois de ordenar.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> rotulos(n);
    long long soma = 0;
    for (auto& r : rotulos) { std::cin >> r; soma += r; }
    std::ranges::sort(rotulos);
    long long repetido = -1;
    for (int i = 1; i < n; ++i)
        if (rotulos[i] == rotulos[i - 1]) repetido = rotulos[i];
    const long long esperada = 1LL * n * (n + 1) / 2;            // produto em 64 bits
    const long long ausente = esperada - soma + repetido;        // A = E - m + d
    std::println("{} {}", repetido, ausente);
}
