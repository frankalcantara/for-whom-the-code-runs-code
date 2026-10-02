// As luzes do painel: lista as posições dos bits ligados de cada máscara, uma iteração por bit ligado.
#include <bit>
#include <cstdint>
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q = 0;
    std::cin >> q;
    std::string saida;
    for (int t = 0; t < q; ++t) {
        std::uint64_t m = 0;
        std::cin >> m;
        if (m == 0) { saida += "apagado\n"; continue; }
        bool primeiro = true;
        while (m != 0) {
            const int pos = std::countr_zero(m);   // posição do bit ligado menos significativo
            if (!primeiro) saida += ' ';
            saida += std::to_string(pos);
            primeiro = false;
            m &= m - 1;                            // desliga esse bit
        }
        saida += '\n';
    }
    std::cout << saida;
}
