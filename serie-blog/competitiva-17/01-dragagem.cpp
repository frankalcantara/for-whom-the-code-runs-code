// T05.1, as equipes de dragagem: m contratos de intervalo como marcas em um vetor de diferenças,
// reconstruído por uma única soma de prefixo.
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<long long> d(n + 2, 0);                 // posições 1..n e a sentinela n + 1
    for (int j = 0; j < m; ++j) {
        int l = 0, r = 0;
        long long x = 0;
        std::cin >> l >> r >> x;
        d[l] += x;                                      // começa a valer em l
        d[r + 1] -= x;                                  // deixa de valer depois de r
    }
    std::string saida;
    long long profundidade = 0;                         // até 2 * 10^5 * 10^9 = 2 * 10^14
    for (int i = 1; i <= n; ++i) {
        profundidade += d[i];
        saida += std::to_string(profundidade);
        saida += (i < n ? ' ' : '\n');
    }
    std::cout << saida;
}
