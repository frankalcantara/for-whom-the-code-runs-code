// T03.1, o contador do telégrafo: quantos pulsos são estritamente mais fortes que o anterior.
#include <print>
#include "leitor.hpp"

int main() {
    static LeitorRapido entrada;                     // 64 KiB fora da pilha
    const long long n = entrada.ler();
    long long anterior = entrada.ler();
    long long subidas = 0;
    for (long long i = 1; i < n; ++i) {
        const long long atual = entrada.ler();
        if (atual > anterior) ++subidas;             // estritamente maior: iguais não contam
        anterior = atual;                            // só o valor anterior importa: nada é guardado
    }
    std::println("{}", subidas);
}
