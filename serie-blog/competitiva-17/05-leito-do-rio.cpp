// T05.5, o levantamento do leito do rio: mínimo de intervalo com sparse table em um vetor plano,
// para até 10^6 consultas, com a saída acumulada em uma única cadeia.
#include <algorithm>
#include <bit>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    const int niveis = std::bit_width(static_cast<unsigned>(n));
    std::vector<int> st(static_cast<std::size_t>(niveis) * n);     // nível j começa em j * n
    for (int i = 0; i < n; ++i) std::cin >> st[i];
    for (int j = 1; j < niveis; ++j) {
        const int meio = 1 << (j - 1);
        int* atual = st.data() + static_cast<std::size_t>(j) * n;
        const int* anterior = st.data() + static_cast<std::size_t>(j - 1) * n;
        for (int i = 0; i + (1 << j) <= n; ++i) atual[i] = std::min(anterior[i], anterior[i + meio]);
    }
    std::string saida;
    saida.reserve(static_cast<std::size_t>(q) * 11);
    for (int t = 0; t < q; ++t) {
        int l = 0, r = 0;
        std::cin >> l >> r;
        --l; --r;
        const int j = std::bit_width(static_cast<unsigned>(r - l + 1)) - 1;
        const int* nivel = st.data() + static_cast<std::size_t>(j) * n;
        saida += std::to_string(std::min(nivel[l], nivel[r - (1 << j) + 1]));
        saida += '\n';
    }
    std::cout << saida;
}
