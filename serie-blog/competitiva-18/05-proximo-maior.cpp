// Próximo maior à direita: para cada posição, o primeiro valor estritamente maior depois dela, com uma
// pilha monotônica de índices pendentes.
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> a(n), resposta(n, -1);
    for (auto& x : a) std::cin >> x;
    std::vector<int> pilha;                             // índices pendentes, valores não crescentes da base ao topo
    pilha.reserve(n);
    for (int i = 0; i < n; ++i) {
        while (!pilha.empty() && a[pilha.back()] < a[i]) {
            resposta[pilha.back()] = a[i];              // a[i] é o primeiro maior para o topo
            pilha.pop_back();
        }
        pilha.push_back(i);
    }
    std::string saida;                                  // quem sobrou na pilha fica com -1
    for (int i = 0; i < n; ++i) { saida += std::to_string(resposta[i]); saida += (i + 1 < n ? ' ' : '\n'); }
    std::cout << saida;
}
