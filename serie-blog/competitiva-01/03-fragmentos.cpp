// Fragmentos do Artigo 1 reunidos em um programa de teste.
// Cada fragmento aparece no artigo exatamente como abaixo.
#include <algorithm>
#include <print>
#include <ranges>
#include <unordered_set>
#include <vector>

void contagem_regressiva(int n) {
    if (n == 0) return;          // caso base: nenhuma chamada nova
    contagem_regressiva(n - 1);  // uma única chamada, com n reduzido em 1
}

void dois_caminhos(int n) {
    if (n == 0) return;
    dois_caminhos(n - 1);  // primeira chamada
    dois_caminhos(n - 1);  // segunda chamada, com o mesmo argumento
}

void metade(long long n) {
    if (n <= 1) return;
    metade(n / 2);
}

int contar_distintos_ingenuo(const std::vector<int>& a) {
    int contador = 0;
    const int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ++i) {
        bool visto = false;
        // Só o prefixo pode desqualificar a posição i. Repetições posteriores
        // pertencem a iterações futuras e não apagam o primeiro representante.
        for (int j = 0; j < i; ++j)
            if (a[j] == a[i]) { visto = true; break; }
        if (!visto) ++contador;
    }
    return contador;
}

int contar_distintos_ordenando(std::vector<int> a) {  // cópia: a ordenação é destrutiva
    std::ranges::sort(a);
    // unique devolve o sufixo descartado; seu início é o fim dos representantes.
    const auto descartados = std::ranges::unique(a);
    return static_cast<int>(std::ranges::distance(a.begin(), descartados.begin()));
}

int contar_distintos_hash(const std::vector<int>& a) {
    std::unordered_set<int> vistos;
    vistos.reserve(a.size());  // evita medir o crescimento repetido da tabela
    for (int x : a) vistos.insert(x);
    return static_cast<int>(vistos.size());
}

int main() {
    const std::vector<int> selos{41, 7, 41, 9, 7, 12, 9, 9};
    std::println("distintos: ingenuo = {}, ordenando = {}, hash = {}",
                 contar_distintos_ingenuo(selos), contar_distintos_ordenando(selos),
                 contar_distintos_hash(selos));
    contagem_regressiva(1'000);
    dois_caminhos(10);
    metade(1'000'000'000);
    std::println("recursoes: ok");
}
