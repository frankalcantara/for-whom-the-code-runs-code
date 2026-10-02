// Soma e mínimo de intervalo: prefixos para a soma e tabela esparsa para o mínimo.
#include <algorithm>
#include <bit>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<long long> a(n), prefixo(n + 1, 0);
    for (int i = 0; i < n; ++i) { std::cin >> a[i]; prefixo[i + 1] = prefixo[i] + a[i]; }
    const int niveis = std::bit_width(static_cast<unsigned>(n));
    std::vector<std::vector<long long>> st(niveis);
    st[0] = a;
    for (int k = 1; k < niveis; ++k) {
        st[k].resize(n - (1 << k) + 1);
        for (int i = 0; i + (1 << k) <= n; ++i) st[k][i] = std::min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
    }
    std::string saida;
    while (q-- > 0) {
        int l = 0, r = 0;                             // posições contadas a partir de 0, intervalo fechado
        std::cin >> l >> r;
        const int k = std::bit_width(static_cast<unsigned>(r - l + 1)) - 1;
        saida += std::to_string(prefixo[r + 1] - prefixo[l]);
        saida.push_back(' ');
        saida += std::to_string(std::min(st[k][l], st[k][r - (1 << k) + 1]));
        saida.push_back('\n');
    }
    std::print("{}", saida);
}
