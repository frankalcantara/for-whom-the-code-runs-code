// T02.6, a lagoa de sal: totais por antidiagonal, sem guardar a grade.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int r = 0, c = 0;
    std::cin >> r >> c;
    std::vector<long long> balde(r + c - 1, 0);      // antidiagonais d = i + j, de 0 a r + c - 2
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j) {
            long long producao = 0;
            std::cin >> producao;
            balde[i + j] += producao;
        }
    int melhor = 0;
    for (int d = 1; d < static_cast<int>(balde.size()); ++d)
        if (balde[d] > balde[melhor]) melhor = d;    // estritamente maior: o menor índice vence o empate
    std::println("{} {}", melhor, balde[melhor]);
}
