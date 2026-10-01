// A estufa de mudas: diferenças em duas dimensões, quatro marcas por retângulo e prefixos nas duas direções.
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int R = 0, C = 0, m = 0;
    std::cin >> R >> C >> m;
    const int largura = C + 1;                        // vetor plano de (R + 1) x (C + 1), com sentinelas
    std::vector<long long> d(static_cast<std::size_t>(R + 1) * largura, 0);
    auto em = [&](int r, int c) -> long long& { return d[static_cast<std::size_t>(r) * largura + c]; };
    for (int k = 0; k < m; ++k) {
        int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
        long long x = 0;
        std::cin >> r1 >> c1 >> r2 >> c2 >> x;        // cantos contados a partir de 1
        --r1; --c1;                                   // r2 e c2 já apontam para depois do retângulo
        em(r1, c1) += x;
        em(r1, c2) -= x;
        em(r2, c1) -= x;
        em(r2, c2) += x;                              // a região subtraída duas vezes volta uma vez
    }
    for (int r = 0; r < R; ++r)
        for (int c = 1; c < C; ++c) em(r, c) += em(r, c - 1);
    for (int r = 1; r < R; ++r)
        for (int c = 0; c < C; ++c) em(r, c) += em(r - 1, c);
    std::string saida;
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (c > 0) saida.push_back(' ');
            saida += std::to_string(em(r, c));
        }
        saida.push_back('\n');
    }
    std::print("{}", saida);
}
