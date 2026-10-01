// T04.2, as fogueiras de vigia: busca na resposta, forma de maximização, com teste guloso.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

// Cabem k fogueiras com espaçamento mínimo d? Acende a primeira e depois sempre a primeira a d ou mais.
bool cabe(const std::vector<long long>& p, int k, long long d) {
    int acesas = 1;
    long long ultima = p[0];
    for (std::size_t i = 1; i < p.size() && acesas < k; ++i)
        if (p[i] - ultima >= d) {
            ++acesas;
            ultima = p[i];
        }
    return acesas >= k;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<long long> p(n);
    for (auto& x : p) std::cin >> x;
    std::ranges::sort(p);
    long long lo = 0;                                 // d = 0 sempre cabe
    long long hi = p[n - 1] - p[0];                   // nenhum espaçamento passa da extensão
    while (lo < hi) {
        const long long meio = lo + (hi - lo + 1) / 2;   // arredonda para cima: último verdadeiro
        if (cabe(p, k, meio)) lo = meio;
        else hi = meio - 1;
    }
    std::println("{}", lo);
}
