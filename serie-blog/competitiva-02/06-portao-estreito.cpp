// Exercício: o portão estreito. Conta os valores que cabem em int e soma todos.
#include <iostream>
#include <limits>
#include <print>

int main() {
    int n = 0;
    std::cin >> n;
    int aceitos = 0;
    long long soma = 0;
    for (int i = 0; i < n; ++i) {
        long long valor = 0;   // lido em 64 bits: nada se perde antes do teste
        std::cin >> valor;
        soma += valor;         // o novo livro registra todos os valores
        if (valor >= std::numeric_limits<int>::min() &&
            valor <= std::numeric_limits<int>::max())
            ++aceitos;
    }
    std::println("{} {}", aceitos, soma);
}
