// Exercício 5.5, o vale congelado: mínimo de intervalo com sparse table em um vetor plano.
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
    const int niveis = std::bit_width(static_cast<unsigned>(n));
    std::vector<int> st(static_cast<std::size_t>(niveis) * n);          // nível k começa em k * n
    for (int i = 0; i < n; ++i) std::cin >> st[i];
    for (int k = 1; k < niveis; ++k) {
        const int* ant = st.data() + static_cast<std::size_t>(k - 1) * n;
        int* cur = st.data() + static_cast<std::size_t>(k) * n;
        const int meio = 1 << (k - 1);
        for (int i = 0; i + (1 << k) <= n; ++i) cur[i] = std::min(ant[i], ant[i + meio]);
    }
    std::string saida;
    while (q-- > 0) {
        int l = 0, r = 0;
        std::cin >> l >> r;
        --l; --r;
        const int k = std::bit_width(static_cast<unsigned>(r - l + 1)) - 1;     // floor(log2(comprimento))
        const int* nivel = st.data() + static_cast<std::size_t>(k) * n;
        saida += std::to_string(std::min(nivel[l], nivel[r - (1 << k) + 1]));
        saida.push_back('\n');
    }
    std::print("{}", saida);
}
