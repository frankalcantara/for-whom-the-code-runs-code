// T01.1, a guardiã da cisterna: primeiro dia em que a soma acumulada passa de C.
#include <iostream>
#include <print>

int main() {
    int n = 0;
    long long marca = 0;           // C chega a 10^18: precisa de 64 bits
    std::cin >> n >> marca;

    long long armazenado = 0;      // soma dos prefixos, até 10^6 * 10^12 = 10^18
    int dia_da_cheia = -1;
    for (int dia = 1; dia <= n; ++dia) {
        long long chegada = 0;
        std::cin >> chegada;
        armazenado += chegada;
        if (dia_da_cheia == -1 && armazenado > marca) dia_da_cheia = dia;
    }
    std::println("{}", dia_da_cheia);
}
