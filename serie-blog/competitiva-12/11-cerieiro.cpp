// A2.4, os caixotes do cerieiro: ordenar, prefixos e upper_bound por orçamento.
#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> preco(n);
    for (auto& p : preco) std::cin >> p;
    std::ranges::sort(preco);
    std::vector<long long> prefixo(n + 1, 0);         // até 2 * 10^5 * 10^9 = 2 * 10^14
    for (int i = 0; i < n; ++i) prefixo[i + 1] = prefixo[i] + preco[i];
    std::string saida;
    while (q-- > 0) {
        long long orcamento = 0;                      // até 10^18
        std::cin >> orcamento;
        const auto it = std::ranges::upper_bound(prefixo, orcamento);   // primeiro prefixo acima do orçamento
        saida += std::to_string((it - prefixo.begin()) - 1);
        saida.push_back('\n');
    }
    std::print("{}", saida);
}
