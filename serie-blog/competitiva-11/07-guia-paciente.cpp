// Exercício 4.3, o guia paciente: só os recordes da direita para a esquerda recebem tempo de descanso.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

struct Mirante {
    long long distancia;
    long long valor;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    long long L = 0, r_grupo = 0, r_guia = 0;
    int k = 0;
    std::cin >> L >> k >> r_grupo >> r_guia;
    std::vector<Mirante> mirantes(k);
    for (auto& m : mirantes) std::cin >> m.distancia >> m.valor;

    // Recordes: mirantes cujo valor supera o de todos os que estão à direita.
    std::vector<Mirante> recordes;
    long long melhor_a_direita = 0;                    // todos os valores são pelo menos 1
    for (int i = k - 1; i >= 0; --i) {
        if (mirantes[i].valor > melhor_a_direita) {
            recordes.push_back(mirantes[i]);
            melhor_a_direita = mirantes[i].valor;
        }
    }
    std::ranges::reverse(recordes);

    const long long folga_por_metro = r_grupo - r_guia;
    long long pontos = 0, anterior = 0;
    for (const auto& m : recordes) {
        pontos += (m.distancia - anterior) * folga_por_metro * m.valor;   // folga nova, gasta toda aqui
        anterior = m.distancia;
    }
    std::println("{}", pontos);
}
