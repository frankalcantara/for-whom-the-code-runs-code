// T02.2, os terraços do vinhedo: somas de retângulos por prefixos em duas dimensões.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int r = 0, c = 0, q = 0;
    std::cin >> r >> c >> q;
    // P[i][j] soma as i primeiras linhas e as j primeiras colunas; linha e coluna 0 valem zero.
    std::vector<std::vector<long long>> P(r + 1, std::vector<long long>(c + 1, 0));
    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= c; ++j) {
            long long producao = 0;
            std::cin >> producao;
            P[i][j] = producao + P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1];
        }
    while (q-- > 0) {
        int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
        std::cin >> r1 >> c1 >> r2 >> c2;
        std::println("{}", P[r2][c2] - P[r1 - 1][c2] - P[r2][c1 - 1] + P[r1 - 1][c1 - 1]);
    }
}
