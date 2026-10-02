// T05.4, a carga do carregador: a maior sequência contígua de caixotes com peso total até W,
// por dois ponteiros, válidos porque todos os pesos são positivos.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    long long W = 0;
    std::cin >> n >> W;
    std::vector<long long> w(n);
    for (auto& x : w) std::cin >> x;
    long long soma = 0;                                 // nunca passa de W + 10^9 < 2 * 10^18
    int l = 0, melhor = 0;
    for (int r = 0; r < n; ++r) {
        soma += w[r];
        while (soma > W) soma -= w[l++];                // l pode passar de r: janela vazia, soma 0
        if (r - l + 1 > melhor) melhor = r - l + 1;
    }
    std::println("{}", melhor);
}
