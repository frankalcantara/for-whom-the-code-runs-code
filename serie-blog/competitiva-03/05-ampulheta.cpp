// T01.5, a ampulheta de manivela: se par, cai a metade; se ímpar, cai um grão.
#include <cstdint>
#include <iostream>
#include <print>

int main() {
    int q = 0;
    std::cin >> q;
    while (q-- > 0) {
        std::uint64_t graos = 0;
        std::cin >> graos;
        int voltas = 0;
        // No máximo 2 * floor(log2 g) + 1 voltas: a simulação é o algoritmo certo.
        while (graos > 0) {
            if (graos % 2 == 0) graos /= 2;
            else                graos -= 1;
            ++voltas;
        }
        std::println("{}", voltas);
    }
}
