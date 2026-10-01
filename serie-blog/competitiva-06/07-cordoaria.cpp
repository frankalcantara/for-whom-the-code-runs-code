// T02.7, o livro da cordoaria: trechos de soma zero como pares de prefixos iguais.
#include <iostream>
#include <print>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::unordered_map<long long, long long> vistos;  // prefixos até 2 * 10^14 em valor absoluto
    vistos.reserve(static_cast<std::size_t>(n) + 1);
    vistos[0] = 1;                                    // P_0 = 0: âncora dos trechos que começam no dia 1
    long long prefixo = 0, serenos = 0;               // a resposta chega a n(n+1)/2, cerca de 2 * 10^10
    for (int dia = 1; dia <= n; ++dia) {
        long long valor = 0;
        std::cin >> valor;
        prefixo += valor;
        auto& ocorrencias = vistos[prefixo];
        serenos += ocorrencias;                       // um trecho para cada prefixo igual anterior
        ++ocorrencias;
    }
    std::println("{}", serenos);
}
