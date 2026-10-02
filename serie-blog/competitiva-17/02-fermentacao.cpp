// T05.2, o diário da fermentação: a menor oscilação, máximo menos mínimo, entre as janelas de k dias,
// com dois deques monotônicos sobre vetores.
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<int> t(n);
    for (auto& x : t) std::cin >> x;
    std::vector<int> qmax(n), qmin(n);                  // índices; cada um entra uma única vez
    int fmax = 0, bmax = 0, fmin = 0, bmin = 0;         // frente e fundo de cada deque
    long long melhor = -1;
    for (int i = 0; i < n; ++i) {
        while (bmax > fmax && t[qmax[bmax - 1]] <= t[i]) --bmax;   // valores decrescentes
        qmax[bmax++] = i;
        while (bmin > fmin && t[qmin[bmin - 1]] >= t[i]) --bmin;   // valores crescentes
        qmin[bmin++] = i;
        if (qmax[fmax] <= i - k) ++fmax;                // saiu da janela
        if (qmin[fmin] <= i - k) ++fmin;
        if (i >= k - 1) {
            const long long oscilacao = static_cast<long long>(t[qmax[fmax]]) - t[qmin[fmin]];   // até 2 * 10^9
            if (melhor < 0 || oscilacao < melhor) melhor = oscilacao;
        }
    }
    std::println("{}", melhor);
}
