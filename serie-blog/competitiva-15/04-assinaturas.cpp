// Exercício 5.7, as assinaturas do observatório: agrupa as máscaras pela largura e soma os bits ligados.
#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <print>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::array<long long, 65> quantidade{};          // larguras de 0 a 64: o índice é a própria largura
    std::array<long long, 65> luzes{};
    for (int i = 0; i < n; ++i) {
        std::uint64_t sinal = 0;
        std::cin >> sinal;
        const int largura = std::bit_width(sinal);    // 0 para o sinal 0
        ++quantidade[largura];
        luzes[largura] += std::popcount(sinal);
    }
    for (int w = 0; w <= 64; ++w)
        if (quantidade[w] > 0) std::println("{} {} {}", w, quantidade[w], luzes[w]);
}
