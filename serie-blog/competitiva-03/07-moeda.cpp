// T01.7, a moeda do usurpador: elemento majoritário por cancelamento de pares.
#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<int> marca(n);
    for (auto& x : marca) std::cin >> x;

    // Primeira passada: o único candidato possível a maioria.
    int candidato = marca[0];
    int peso = 0;
    for (int x : marca) {
        if (peso == 0) { candidato = x; peso = 1; }
        else if (x == candidato) ++peso;
        else --peso;
    }

    // Segunda passada: o cancelamento só prova que o candidato PODE ser a maioria.
    long long contagem = 0;
    for (int x : marca)
        if (x == candidato) ++contagem;

    if (2 * contagem > n) std::println("{}", candidato);
    else                  std::println("NONE");
}
