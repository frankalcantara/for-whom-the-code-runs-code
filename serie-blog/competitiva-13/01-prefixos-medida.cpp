// Construção dos prefixos e custo das consultas: quatro laços de construção e o efeito do cache nas consultas.
#include <algorithm>
#include <numeric>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

int main() {
    std::mt19937 gerador(20261001);
    long long obs = 0;
    auto nada = [] {};

    // 1. Construção de 10^7 prefixos, escrita de quatro formas.
    const int n = 10'000'000;
    std::uniform_int_distribution<int> valor(-1'000'000'000, 1'000'000'000);
    std::vector<int> a(n);
    for (int& x : a) x = valor(gerador);
    std::vector<long long> p1(n + 1), p2(n + 1), p3(n + 1), p4(n + 1);
    const auto lendo = medir_com_preparo(nada, [&] {
        p1[0] = 0;
        for (int i = 0; i < n; ++i) p1[i + 1] = p1[i] + a[i];          // relê p1[i], gravado na iteração anterior
        return p1[n];
    }, obs);
    const auto local = medir_com_preparo(nada, [&] {
        long long soma = 0;                                            // acumulador em variável local
        p2[0] = 0;
        for (int i = 0; i < n; ++i) { soma += a[i]; p2[i + 1] = soma; }
        return p2[n];
    }, obs);
    const auto ponteiros = medir_com_preparo(nada, [&] {
        const int* origem = a.data();                                  // ponteiros copiados para variáveis locais
        long long* destino = p3.data();
        long long soma = 0;
        destino[0] = 0;
        for (int i = 0; i < n; ++i) { soma += origem[i]; destino[i + 1] = soma; }
        return p3[n];
    }, obs);
    const auto scan = medir_com_preparo(nada, [&] {
        p4[0] = 0;
        std::inclusive_scan(a.begin(), a.end(), p4.begin() + 1, std::plus<>{}, 0LL);   // acumulador long long
        return p4[n];
    }, obs);
    std::println("construção de 10^7 prefixos: relendo p[i] {:.1f} ms, acumulador local {:.1f} ms, "
                 "ponteiros locais {:.1f} ms, inclusive_scan {:.1f} ms, iguais: {}",
                 lendo.mediana_ms, local.mediana_ms, ponteiros.mediana_ms, scan.mediana_ms,
                 p1 == p2 && p2 == p3 && p3 == p4);

    // 2. Soma direta contra prefixos para m = 10^5 valores e 10^4 intervalos aleatórios.
    {
        const int m = 100'000, consultas = 10'000;
        std::vector<int> b(a.begin(), a.begin() + m);
        std::vector<long long> pb(m + 1, 0);
        for (int i = 0; i < m; ++i) pb[i + 1] = pb[i] + b[i];
        std::uniform_int_distribution<int> pos(0, m - 1);
        std::vector<std::pair<int, int>> intervalos(consultas);
        long long celulas = 0;
        for (auto& [l, r] : intervalos) {
            l = pos(gerador); r = pos(gerador);
            if (l > r) std::swap(l, r);
            celulas += r - l + 1;
        }
        long long s1 = 0, s2 = 0;
        const auto direta = medir_com_preparo(nada, [&] {
            s1 = 0;
            for (auto [l, r] : intervalos) for (int i = l; i <= r; ++i) s1 += b[i];
            return s1;
        }, obs);
        const auto prefixo = medir_com_preparo(nada, [&] {
            s2 = 0;
            for (auto [l, r] : intervalos) s2 += pb[r + 1] - pb[l];
            return s2;
        }, obs);
        std::println("10^4 consultas em 10^5 valores: soma direta {:.2f} ms ({} parcelas), prefixos {:.4f} ms, iguais: {}",
                     direta.mediana_ms, celulas, prefixo.mediana_ms, s1 == s2);
    }

    // 3. Custo por consulta aleatória conforme o tamanho do vetor de prefixos.
    const int consultas = 10'000'000;
    for (int m : {10'000, 100'000, 1'000'000, 10'000'000}) {
        std::uniform_int_distribution<int> pos(0, m - 1);
        std::vector<std::pair<int, int>> intervalos(consultas);
        for (auto& [l, r] : intervalos) {
            l = pos(gerador); r = pos(gerador);
            if (l > r) std::swap(l, r);
        }
        const auto t = medir_com_preparo(nada, [&] {
            long long s = 0;
            for (auto [l, r] : intervalos) s += p4[r + 1] - p4[l];
            return s;
        }, obs);
        std::println("prefixos de {:>8} valores ({:>6.2f} MB): {:.2f} ns por consulta aleatória", m,
                     (m + 1) * 8.0 / 1e6, t.mediana_ms * 1e6 / consultas);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
