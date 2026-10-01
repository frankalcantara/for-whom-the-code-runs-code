// O mesmo ranking com uma tabela de contagem das pontuações: O(n + S), sem busca e sem ordenar as pontuações.
#include <algorithm>
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    constexpr int S = 100'001;                         // pontuações de 0 a 100 000
    int n = 0, k = 0, limiar = 0;
    std::cin >> n >> k >> limiar;
    std::vector<int> pontos(n);
    std::vector<std::string> nomes(n);
    std::vector<int> cont(S + 1, 0);                   // cont[x]: competidores com pontuação x
    for (int i = 0; i < n; ++i) {
        int id = 0;
        std::cin >> id >> pontos[i] >> nomes[i];
        ++cont[pontos[i]];
    }
    // Sufixos: acima[x] = competidores com pontuação >= x. A posição S vale zero.
    std::vector<int> acima(S + 1, 0);
    for (int x = S - 1; x >= 0; --x) acima[x] = acima[x + 1] + cont[x];

    int alvo = S - 1;
    while (acima[alvo] < k) --alvo;                    // maior x com acima[x] >= k

    std::vector<std::string> escolhidos;
    for (int i = 0; i < n; ++i)
        if (pontos[i] == alvo) escolhidos.push_back(nomes[i]);
    std::ranges::sort(escolhidos);
    std::string linha;
    for (const auto& nome : escolhidos) {
        if (!linha.empty()) linha += ' ';
        linha += nome;
    }
    std::println("{}", alvo);
    std::println("{}", linha);
    std::println("{}", limiar + 1 <= S - 1 ? acima[limiar + 1] : 0);
}
