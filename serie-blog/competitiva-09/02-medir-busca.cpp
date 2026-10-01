// Custo de uma busca: linear contra binária, e o efeito do tamanho do arranjo sobre a busca binária.
#include <algorithm>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

int primeiro_ge(const std::vector<int>& v, int alvo) {
    int lo = 0, hi = static_cast<int>(v.size());
    while (lo < hi) {
        const int meio = lo + (hi - lo) / 2;
        if (v[meio] < alvo) lo = meio + 1;
        else                hi = meio;
    }
    return lo;
}

// A mesma busca sem desvio condicional: o intervalo é reduzido à metade com uma seleção,
// que o compilador pode traduzir por uma instrução de movimento condicional.
int primeiro_ge_sem_desvio(const std::vector<int>& v, int alvo) {
    if (v.empty()) return 0;
    const int* base = v.data();
    std::size_t n = v.size();
    while (n > 1) {
        const std::size_t metade = n / 2;
        base = base[metade] < alvo ? base + metade : base;   // invariante: a resposta está em [base, base + n]
        n -= metade;
    }
    return static_cast<int>(base - v.data()) + (*base < alvo);
}

int main() {
    std::mt19937 gerador(20260930);
    long long obs = 0;

    // 1. Linear contra binária, n = 10^5, 10^4 consultas.
    {
        const int n = 100'000, q = 10'000;
        std::vector<int> v(n);
        for (int i = 0; i < n; ++i) v[i] = 3 * i;
        std::uniform_int_distribution<int> dist(0, 3 * n);
        std::vector<int> alvos(q);
        for (int& a : alvos) a = dist(gerador);
        auto linear = [&] {
            long long s = 0;
            for (int a : alvos) {
                int i = 0;
                while (i < n && v[i] < a) ++i;
                s += i;
            }
            return s;
        };
        auto binaria = [&] { long long s = 0; for (int a : alvos) s += primeiro_ge(v, a); return s; };
        const auto ml = medir(linear, obs), mb = medir(binaria, obs);
        std::println("n = 10^5, 10^4 consultas: linear {:.3f} ms, binária {:.3f} ms, respostas iguais: {}",
                     ml.mediana_ms, mb.mediana_ms, linear() == binaria());
    }

    // 2. Busca binária com 10^6 consultas aleatórias, para arranjos de 2^10 a 2^24 inteiros.
    std::println("{:>10} | {:>8} | {:>12} | {:>15} | {:>12}", "n", "KiB", "manual (ns)", "biblioteca (ns)", "sem desvio (ns)");
    for (int expo : {10, 13, 16, 19, 22, 24}) {
        const int n = 1 << expo;
        std::vector<int> v(n);
        for (int i = 0; i < n; ++i) v[i] = 2 * i;
        std::uniform_int_distribution<int> dist(0, 2 * n);
        std::vector<int> alvos(1'000'000);
        for (int& a : alvos) a = dist(gerador);
        auto manual = [&] { long long s = 0; for (int a : alvos) s += primeiro_ge(v, a); return s; };
        auto biblioteca = [&] {
            long long s = 0;
            for (int a : alvos) s += std::ranges::lower_bound(v, a) - v.begin();
            return s;
        };
        auto sem_desvio = [&] { long long s = 0; for (int a : alvos) s += primeiro_ge_sem_desvio(v, a); return s; };
        const auto mm = medir(manual, obs), mbib = medir(biblioteca, obs), msd = medir(sem_desvio, obs);
        if (manual() != biblioteca() || manual() != sem_desvio()) std::println("divergência em n = {}", n);
        // 10^6 consultas: milissegundos totais equivalem a nanossegundos por consulta
        std::println("{:>10} | {:>8} | {:>12.1f} | {:>15.1f} | {:>12.1f}", n, n * 4 / 1024,
                     mm.mediana_ms, mbib.mediana_ms, msd.mediana_ms);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
