// Atualização incremental: a soma das leituras pares depois de cada ajuste, sem reler o vetor.
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> umidade(n);
    long long soma_pares = 0;                         // até 2 * 10^5 valores de módulo até 2 * 10^14
    for (auto& u : umidade) {
        std::cin >> u;
        if (u % 2 == 0) soma_pares += u;
    }
    std::string saida;
    while (q-- > 0) {
        long long ajuste = 0;
        int idx = 0;
        std::cin >> ajuste >> idx;
        long long& u = umidade[idx - 1];
        if (u % 2 == 0) soma_pares -= u;              // retira a contribuição antiga
        u += ajuste;
        if (u % 2 == 0) soma_pares += u;              // acrescenta a nova
        saida += std::to_string(soma_pares);
        saida.push_back('\n');
    }
    std::print("{}", saida);
}
