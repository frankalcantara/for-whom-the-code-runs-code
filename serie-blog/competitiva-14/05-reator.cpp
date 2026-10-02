// Exercício 5.2, os ciclos do reator: menor e maior soma entre as janelas de tamanho w.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, w = 0;
    std::cin >> n >> w;
    std::vector<long long> energia(n);
    for (auto& e : energia) std::cin >> e;
    long long janela = 0;                             // até 2 * 10^14 em valor absoluto
    for (int i = 0; i < w; ++i) janela += energia[i];
    long long menor = janela, maior = janela;
    for (int direita = w; direita < n; ++direita) {
        janela += energia[direita];
        janela -= energia[direita - w];
        menor = std::min(menor, janela);
        maior = std::max(maior, janela);
    }
    std::println("{} {}", menor, maior);
}
