// O limite inferior da ordenação por comparação e as comparações que o
// std::ranges::sort e o std::ranges::stable_sort realmente fazem.
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <print>
#include <random>
#include <vector>

long long comparacoes = 0;

// Comparação que conta as próprias chamadas.
struct MenorContado {
    bool operator()(int a, int b) const { ++comparacoes; return a < b; }
};

// ceil(log2(n!)) pela soma dos logaritmos, exata para os n usados aqui.
long long piso_informacao(long long n) {
    if (n <= 1) return 0;
    const double bits = std::lgamma(static_cast<double>(n) + 1.0) / std::log(2.0);
    return static_cast<long long>(std::ceil(bits - 1e-9));
}

int main() {
    // 1. Os seis arranjos de três elementos distintos.
    std::array<int, 3> p{1, 2, 3};
    int pior = 0;
    std::print("três elementos:");
    do {
        auto v = std::vector<int>(p.begin(), p.end());
        comparacoes = 0;
        std::ranges::sort(v, MenorContado{});
        pior = std::max(pior, static_cast<int>(comparacoes));
        std::print(" {}{}{}:{}", p[0], p[1], p[2], comparacoes);
    } while (std::ranges::next_permutation(p).found);
    std::println("");
    std::println("pior caso do sort com 3 elementos: {} comparações, limite inferior {}", pior, piso_informacao(3));

    // 2. Comparações médias em permutações aleatórias.
    std::println("{:>9} | {:>14} | {:>14} | {:>14} | {:>14}", "n", "ceil(log2 n!)", "n log2 n", "sort", "stable_sort");
    std::mt19937 gerador(20260930);
    for (int n : {10, 100, 1'000, 10'000, 100'000, 1'000'000}) {
        std::vector<int> base(n);
        std::iota(base.begin(), base.end(), 0);
        std::ranges::shuffle(base, gerador);
        auto v = base;
        comparacoes = 0;
        std::ranges::sort(v, MenorContado{});
        const long long c_sort = comparacoes;
        const bool ok1 = std::ranges::is_sorted(v);
        v = base;
        comparacoes = 0;
        std::ranges::stable_sort(v, MenorContado{});
        const long long c_estavel = comparacoes;
        const bool ok2 = std::ranges::is_sorted(v);
        const double nlogn = n * std::log2(static_cast<double>(n));
        std::println("{:>9} | {:>14} | {:>14.0f} | {:>14} | {:>14}{}", n, piso_informacao(n), nlogn, c_sort, c_estavel,
                     ok1 && ok2 ? "" : "  ERRO");
    }
}
