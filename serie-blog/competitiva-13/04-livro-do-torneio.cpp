// Exercício 5.1, o livro do torneio: prefixos de contagem para três categorias.
#include <array>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::string s;
    std::cin >> n >> q >> s;
    std::vector<std::array<int, 3>> prefixo(n + 1);   // linha 0: o prefixo vazio, tudo zero
    for (int i = 1; i <= n; ++i) {
        prefixo[i] = prefixo[i - 1];
        const char g = s[i - 1];
        ++prefixo[i][g == 'R' ? 0 : g == 'P' ? 1 : 2];
    }
    std::string saida;
    while (q-- > 0) {
        int l = 0, r = 0;
        std::cin >> l >> r;
        for (int g = 0; g < 3; ++g) {
            saida += std::to_string(prefixo[r][g] - prefixo[l - 1][g]);
            saida.push_back(g < 2 ? ' ' : '\n');
        }
    }
    std::print("{}", saida);
}
