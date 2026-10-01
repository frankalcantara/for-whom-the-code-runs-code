// Exercício 4.7, os carimbos de tinta: cada faixa preta máxima precisa de ceil(l / w) carimbos.
#include <iostream>
#include <print>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int linhas = 0, colunas = 0, w = 0;
    std::cin >> linhas >> colunas >> w;
    long long carimbos = 0;
    bool possivel = true;
    std::string folha;
    for (int r = 0; r < linhas; ++r) {
        std::cin >> folha;
        for (int esq = 0; esq < colunas;) {
            if (folha[esq] == 'W') { ++esq; continue; }
            int dir = esq;
            while (dir < colunas && folha[dir] == 'B') ++dir;     // [esq, dir) é uma faixa preta máxima
            const int l = dir - esq;
            if (l < w) possivel = false;
            else carimbos += (l + w - 1) / w;
            esq = dir;
        }
    }
    if (possivel) std::println("{}", carimbos);
    else std::println("-1");
}
