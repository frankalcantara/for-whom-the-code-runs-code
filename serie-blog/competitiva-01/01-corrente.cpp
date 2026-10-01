// Percurso em profundidade de uma corrente: recursão contra pilha explícita.
// Compilar: cl /std:c++latest /O2 /EHsc corrente.cpp
//       ou: clang++ -std=c++23 -O2 corrente.cpp -o corrente
#include <cstddef>
#include <print>
#include <vector>

// Grafo em forma de corrente: o vértice v aponta para v + 1.
// Representado pela lista de vizinhos de cada vértice.
using Grafo = std::vector<std::vector<int>>;

Grafo construir_corrente(int n) {
    Grafo g(n);
    for (int v = 0; v + 1 < n; ++v)
        g[v].push_back(v + 1);
    return g;
}

// Versão recursiva: cada nível de profundidade consome um quadro de pilha.
void visitar_recursivo(const Grafo& g, int v, std::vector<char>& visto, long long& soma) {
    visto[v] = 1;
    soma += v;
    for (int w : g[v])
        if (!visto[w]) visitar_recursivo(g, w, visto, soma);
}

// Versão iterativa: o estado da recursão vive em um std::vector no heap.
long long visitar_iterativo(const Grafo& g, int origem) {
    std::vector<char> visto(g.size(), 0);
    std::vector<int> pilha;
    pilha.reserve(g.size());
    pilha.push_back(origem);
    visto[origem] = 1;
    long long soma = 0;
    while (!pilha.empty()) {
        const int v = pilha.back();
        pilha.pop_back();
        soma += v;
        for (int w : g[v]) {
            if (!visto[w]) {
                visto[w] = 1;
                pilha.push_back(w);
            }
        }
    }
    return soma;
}

int main() {
    // Profundidade pequena: as duas versões concordam.
    const Grafo pequeno = construir_corrente(1'000);
    std::vector<char> visto(pequeno.size(), 0);
    long long soma_rec = 0;
    visitar_recursivo(pequeno, 0, visto, soma_rec);
    std::println("n = 1000: recursiva = {}, iterativa = {}",
                 soma_rec, visitar_iterativo(pequeno, 0));

    // Profundidade de um milhão: apenas a versão iterativa é segura.
    const Grafo grande = construir_corrente(1'000'000);
    std::println("n = 1000000: iterativa = {}", visitar_iterativo(grande, 0));
}
