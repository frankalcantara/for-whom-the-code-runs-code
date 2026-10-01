// T03.5, o inspetor do tear: a maior sequência de listras consecutivas de mesma cor, em uma passada.
#include <print>
#include "leitor.hpp"

int main() {
    static LeitorRapido entrada;
    const long long n = entrada.ler();
    long long anterior = entrada.ler();
    long long atual = 1, melhor = 1;                 // atual: comprimento da sequência que termina aqui
    for (long long i = 1; i < n; ++i) {
        const long long cor = entrada.ler();
        atual = cor == anterior ? atual + 1 : 1;
        if (atual > melhor) melhor = atual;          // comparado a cada passo, inclusive no último
        anterior = cor;
    }
    std::println("{}", melhor);
}
