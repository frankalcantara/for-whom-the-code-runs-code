// Exercício 5.8, os registros das constelações: conjuntos em palavras de 64 bits e contagem com std::popcount.
#include <bit>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::size_t universo = 0, registros = 0, perguntas = 0;
    std::cin >> universo >> registros >> perguntas;
    const std::size_t palavras = (universo + 63) / 64;
    std::vector<std::uint64_t> bits(registros * palavras, 0);   // registro r começa em r * palavras
    for (std::size_t r = 0; r < registros; ++r) {
        std::size_t k = 0;
        std::cin >> k;
        for (std::size_t t = 0; t < k; ++t) {
            std::size_t estrela = 0;
            std::cin >> estrela;
            const std::size_t i = estrela - 1;                     // bit lógico, contado a partir de 0
            bits[r * palavras + i / 64] |= std::uint64_t{1} << (i % 64);
        }
    }
    std::string saida;
    for (std::size_t q = 0; q < perguntas; ++q) {
        int tipo = 0;
        std::size_t a = 0, b = 0;
        std::cin >> tipo >> a >> b;
        const std::uint64_t* x = bits.data() + (a - 1) * palavras;
        const std::uint64_t* y = bits.data() + (b - 1) * palavras;
        std::size_t resposta = 0;
        switch (tipo) {                                            // um laço por operação, sem vetor temporário
        case 1: for (std::size_t j = 0; j < palavras; ++j) resposta += std::popcount(x[j] & y[j]); break;
        case 2: for (std::size_t j = 0; j < palavras; ++j) resposta += std::popcount(x[j] | y[j]); break;
        case 3: for (std::size_t j = 0; j < palavras; ++j) resposta += std::popcount(x[j] ^ y[j]); break;
        default: for (std::size_t j = 0; j < palavras; ++j) resposta += std::popcount(x[j] & ~y[j]); break;
        }
        saida += std::to_string(resposta);
        saida += '\n';
    }
    std::cout << saida;
}
