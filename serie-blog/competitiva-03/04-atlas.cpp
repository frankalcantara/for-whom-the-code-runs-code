// T01.4, o atlas estelar: memória de uma grade com 8 bytes por célula, em MiB.
#include <cstdint>
#include <iostream>
#include <print>

int main() {
    constexpr std::uint64_t MIB = 1ULL << 20;
    int q = 0;
    std::cin >> q;
    while (q-- > 0) {
        std::uint64_t r = 0, c = 0, orcamento = 0;
        std::cin >> r >> c >> orcamento;
        // O produto é feito em 64 bits sem sinal desde o primeiro fator:
        // 8 * 10^9 * 10^9 = 8 * 10^18 cabe abaixo de 1,8 * 10^19.
        const std::uint64_t bytes = 8ULL * r * c;
        const std::uint64_t mib = (bytes + MIB - 1) / MIB;   // divisão arredondada para cima
        if (bytes <= orcamento * MIB) std::println("FITS {}", mib);
        else                          std::println("TOO LARGE {}", mib);
    }
}
