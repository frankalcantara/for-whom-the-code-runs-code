// O livro das caravanas: agregados, vinculações estruturadas e desempate por nome.
#include <iostream>
#include <print>
#include <string>
#include <vector>

struct Caravana {
    std::string nome;
    long long caixas;
    long long peso_por_caixa;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<Caravana> caravanas(n);
    for (auto& [nome, caixas, peso] : caravanas) std::cin >> nome >> caixas >> peso;

    long long total_caixas = 0, total_peso = 0;
    const Caravana* vencedora = nullptr;
    long long carga_vencedora = -1;
    for (const auto& caravana : caravanas) {
        const auto& [nome, caixas, peso] = caravana;
        const long long carga = caixas * peso;           // até 10^12
        total_caixas += caixas;
        total_peso += carga;                              // até 2 * 10^17
        if (carga > carga_vencedora || (carga == carga_vencedora && nome < vencedora->nome)) {
            vencedora = &caravana;
            carga_vencedora = carga;
        }
    }
    std::println("{} {}", total_caixas, total_peso);
    std::println("{} {}", vencedora->nome, carga_vencedora);
}
