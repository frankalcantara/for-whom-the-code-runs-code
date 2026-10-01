// Exercício 4.2, o pareamento noturno: ordenar os grupos e parear os extremos.
#include <algorithm>
#include <iostream>
#include <print>
#include <utility>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int g = 0;
    std::cin >> g;
    std::vector<std::pair<long long, long long>> grupos(g);     // (valor, quantidade)
    for (auto& [valor, quantidade] : grupos) std::cin >> quantidade >> valor;
    std::ranges::sort(grupos);                                   // O(g log g), nunca expande as fichas

    int esq = 0, dir = g - 1;
    long long resposta = 0;
    while (esq < dir) {                                          // menor restante com maior restante
        resposta = std::max(resposta, grupos[esq].first + grupos[dir].first);
        const long long pares = std::min(grupos[esq].second, grupos[dir].second);
        grupos[esq].second -= pares;
        grupos[dir].second -= pares;
        if (grupos[esq].second == 0) ++esq;
        if (grupos[dir].second == 0) --dir;
    }
    if (esq == dir && grupos[esq].second > 0)                    // sobra um grupo: pares internos
        resposta = std::max(resposta, 2 * grupos[esq].first);
    std::println("{}", resposta);
}
