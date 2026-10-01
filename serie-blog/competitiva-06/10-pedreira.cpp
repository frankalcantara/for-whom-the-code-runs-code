// A1.3, o mapa da pedreira: melhor bloco k x k por prefixos em duas dimensões.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int r = 0, c = 0, k = 0;
    std::cin >> r >> c >> k;
    std::vector<std::vector<long long>> P(r + 1, std::vector<long long>(c + 1, 0));
    for (int i = 1; i <= r; ++i)
        for (int j = 1; j <= c; ++j) {
            long long qualidade = 0;
            std::cin >> qualidade;
            P[i][j] = qualidade + P[i - 1][j] + P[i][j - 1] - P[i - 1][j - 1];
        }
    int melhor_linha = 1, melhor_coluna = 1;
    long long melhor_total = -1;                      // qualidades não negativas: qualquer bloco vence -1
    for (int i = 1; i + k - 1 <= r; ++i)              // linhas em ordem crescente
        for (int j = 1; j + k - 1 <= c; ++j) {        // colunas em ordem crescente
            const int i2 = i + k - 1, j2 = j + k - 1;
            const long long total = P[i2][j2] - P[i - 1][j2] - P[i2][j - 1] + P[i - 1][j - 1];
            if (total > melhor_total) {               // estritamente maior: o primeiro canto vence o empate
                melhor_total = total;
                melhor_linha = i;
                melhor_coluna = j;
            }
        }
    std::println("{} {} {}", melhor_linha, melhor_coluna, melhor_total);
}
