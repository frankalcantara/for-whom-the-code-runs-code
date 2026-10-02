// O baú do mercador: conta os subconjuntos de até 20 itens com peso total igual ao alvo, uma soma por máscara.
#include <bit>
#include <cstdint>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    long long alvo = 0;
    std::cin >> n >> alvo;
    std::vector<long long> peso(n);
    for (auto& p : peso) std::cin >> p;
    const std::uint32_t total = std::uint32_t{1} << n;          // n <= 20, longe do limite de 32 bits
    std::vector<long long> soma(total, 0);                        // soma[0] = 0, o subconjunto vazio
    long long contagem = (alvo == 0) ? 1 : 0;
    for (std::uint32_t m = 1; m < total; ++m) {
        const int menor = std::countr_zero(m);                    // o item de menor índice da máscara
        soma[m] = soma[m & (m - 1)] + peso[menor];                // m sem esse item já foi calculada
        if (soma[m] == alvo) ++contagem;
    }
    std::println("{}", contagem);
}
