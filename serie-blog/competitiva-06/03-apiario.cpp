// T02.3, o censo do apiário: contagem com tabela de dispersão e desempate pelo menor número.
#include <iostream>
#include <print>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::unordered_map<int, int> freq;               // placas até 10^9: um vetor de contagem é inviável
    freq.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        int placa = 0;
        std::cin >> placa;
        ++freq[placa];
    }
    int melhor_placa = 0, melhor_contagem = 0;
    for (const auto& [placa, contagem] : freq)       // a ordem da tabela é arbitrária
        if (contagem > melhor_contagem || (contagem == melhor_contagem && placa < melhor_placa)) {
            melhor_placa = placa;
            melhor_contagem = contagem;
        }
    std::println("{} {}", melhor_placa, melhor_contagem);
}
