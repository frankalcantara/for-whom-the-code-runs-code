// Exercício 5.10, as prateleiras do boticário: mesmo multiconjunto em dois trechos, por somas de
// prefixo de assinaturas aleatórias de 64 bits, com a aritmética sem sinal módulo 2^64.
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int na = 0, nb = 0, q = 0;
    std::cin >> na >> nb >> q;
    std::mt19937_64 gerador(std::random_device{}());
    std::unordered_map<int, std::uint64_t> assinatura;
    assinatura.reserve(static_cast<std::size_t>(na + nb));
    auto prefixos = [&](int n) {
        std::vector<std::uint64_t> p(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            int id = 0;
            std::cin >> id;
            auto [it, nova] = assinatura.try_emplace(id, 0);
            if (nova) it->second = gerador();                  // sorteada na primeira aparição
            p[i + 1] = p[i] + it->second;                      // overflow de propósito, módulo 2^64
        }
        return p;
    };
    const auto pa = prefixos(na);
    const auto pb = prefixos(nb);
    std::string saida;
    for (int t = 0; t < q; ++t) {
        int l1 = 0, r1 = 0, l2 = 0, r2 = 0;
        std::cin >> l1 >> r1 >> l2 >> r2;
        const bool igual = (r1 - l1) == (r2 - l2) && pa[r1] - pa[l1 - 1] == pb[r2] - pb[l2 - 1];
        saida += igual ? "YES\n" : "NO\n";
    }
    std::cout << saida;
}
