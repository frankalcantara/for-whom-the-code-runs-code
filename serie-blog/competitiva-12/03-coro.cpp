// T04.3, as galerias do coro: contagem em um domínio de 201 alturas.
#include <array>
#include <iostream>
#include <print>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::array<int, 251> cont{};                      // índices 50 a 250 usados
    for (int i = 0; i < n; ++i) {
        int h = 0;
        std::cin >> h;
        ++cont[h];
    }
    std::string saida;                                // até 10^6 números: uma única escrita
    saida.reserve(static_cast<std::size_t>(n) * 4);
    for (int h = 50; h <= 250; ++h)
        for (int c = 0; c < cont[h]; ++c) {
            if (!saida.empty()) saida.push_back(' ');
            saida += std::to_string(h);
        }
    std::println("{}", saida);
}
