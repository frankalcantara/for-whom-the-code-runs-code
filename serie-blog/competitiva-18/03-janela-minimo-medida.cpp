// Mínimo de cada janela de 10^3 em 10^6 valores: deque monotônico sobre vetor, fila com duas pilhas
// e std::multiset.
#include <algorithm>
#include <iterator>
#include <print>
#include <random>
#include <set>
#include <utility>
#include <vector>
#include "medicao.hpp"

int main() {
    std::mt19937_64 gerador(20261018);
    const int n = 1'000'000, k = 1'000;
    std::vector<int> a(n);
    std::uniform_int_distribution<int> valor(-1'000'000'000, 1'000'000'000);
    for (auto& x : a) x = valor(gerador);
    long long obs = 0;
    auto nada = [] {};
    long long r1 = 0, r2 = 0, r3 = 0;
    const auto m1 = medir_com_preparo(nada, [&] {
        std::vector<int> q(n);
        int f = 0, b = 0;
        long long s = 0;
        for (int i = 0; i < n; ++i) {
            while (b > f && a[q[b - 1]] >= a[i]) --b;
            q[b++] = i;
            if (q[f] <= i - k) ++f;
            if (i >= k - 1) s += a[q[f]];
        }
        return r1 = s;
    }, obs);
    const auto m2 = medir_com_preparo(nada, [&] {
        std::vector<std::pair<int, int>> entrada, saida;           // (valor, mínimo até aqui)
        entrada.reserve(k); saida.reserve(k);
        long long s = 0;
        for (int i = 0; i < n; ++i) {
            entrada.emplace_back(a[i], entrada.empty() ? a[i] : std::min(a[i], entrada.back().second));
            if (i >= k) {
                if (saida.empty())
                    while (!entrada.empty()) {
                        const int x = entrada.back().first;
                        entrada.pop_back();
                        saida.emplace_back(x, saida.empty() ? x : std::min(x, saida.back().second));
                    }
                saida.pop_back();
            }
            if (i >= k - 1) {
                int m = entrada.empty() ? saida.back().second : entrada.back().second;
                if (!entrada.empty() && !saida.empty()) m = std::min(entrada.back().second, saida.back().second);
                s += m;
            }
        }
        return r2 = s;
    }, obs);
    const auto m3 = medir_com_preparo(nada, [&] {
        std::multiset<int> janela;
        long long s = 0;
        for (int i = 0; i < n; ++i) {
            janela.insert(a[i]);
            if (i >= k) janela.erase(janela.find(a[i - k]));
            if (i >= k - 1) s += *janela.begin();
        }
        return r3 = s;
    }, obs);
    std::println("deque monotônico sobre vetor: {:.2f} ms", m1.mediana_ms);
    std::println("fila com duas pilhas: {:.2f} ms", m2.mediana_ms);
    std::println("std::multiset: {:.2f} ms", m3.mediana_ms);
    std::println("mesmos mínimos: {}", r1 == r2 && r2 == r3);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
