// O termômetro da trilha: o mínimo de cada janela de k leituras, com um deque de índices sobre um vetor.
#include <iostream>
#include <print>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<int> a(n), fila(n);
    for (auto& x : a) std::cin >> x;
    int frente = 0, fundo = 0;                        // a fila de índices ocupa [frente, fundo)
    std::string saida;
    for (int i = 0; i < n; ++i) {
        if (frente < fundo && fila[frente] <= i - k) ++frente;          // o índice da frente saiu da janela
        while (frente < fundo && a[fila[fundo - 1]] >= a[i]) --fundo;   // dominados por a[i]
        fila[fundo++] = i;
        if (i >= k - 1) {
            if (!saida.empty()) saida.push_back(' ');
            saida += std::to_string(a[fila[frente]]);
        }
    }
    std::println("{}", saida);
}
