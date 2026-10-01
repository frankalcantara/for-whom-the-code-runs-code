// Exercício 5.4, a rua das lanternas: cada operação marca duas fronteiras, e um prefixo reconstrói o brilho.
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<long long> diferenca(n + 1, 0);       // a posição n é a sentinela para r = n
    for (int k = 0; k < m; ++k) {
        int l = 0, r = 0;
        long long x = 0;
        std::cin >> l >> r >> x;
        diferenca[l - 1] += x;                        // o efeito começa na lanterna l
        diferenca[r] -= x;                            // e termina depois da lanterna r
    }
    std::string saida;
    long long brilho = 0;                             // até 2 * 10^5 * 10^9 = 2 * 10^14 em valor absoluto
    for (int i = 0; i < n; ++i) {
        brilho += diferenca[i];
        if (i > 0) saida.push_back(' ');
        saida += std::to_string(brilho);
    }
    std::println("{}", saida);
}
