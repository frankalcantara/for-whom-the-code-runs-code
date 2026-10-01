// Percorrer a mesma matriz por linhas e por colunas, para vários tamanhos.
#include <cstddef>
#include <print>
#include <vector>
#include "medicao.hpp"

int main() {
    std::println("{:>6} {:>10} {:>12} {:>12} {:>8} {:>8}", "n", "MiB", "linhas ms", "colunas ms", "razão", "somas");
    for (const int n : {256, 1024, 2048, 4096}) {
        const std::size_t N = static_cast<std::size_t>(n);
        // Um único bloco contíguo: a célula (r, c) fica no índice r * n + c.
        std::vector<int> m(N * N);
        for (std::size_t i = 0; i < m.size(); ++i) m[i] = static_cast<int>(i % 97);

        auto por_linhas = [&] {
            long long s = 0;
            for (std::size_t r = 0; r < N; ++r)
                for (std::size_t c = 0; c < N; ++c) s += m[r * N + c];
            return s;
        };
        auto por_colunas = [&] {
            long long s = 0;
            for (std::size_t c = 0; c < N; ++c)
                for (std::size_t r = 0; r < N; ++r) s += m[r * N + c];
            return s;
        };
        long long obs = 0;
        const Medicao l = medir(por_linhas, obs);
        const Medicao c = medir(por_colunas, obs);
        const bool iguais = por_linhas() == por_colunas();
        const double mib = static_cast<double>(N * N * sizeof(int)) / (1 << 20);
        std::println("{:>6} {:>10.2f} {:>12.3f} {:>12.3f} {:>8.2f} {:>8}", n, mib,
                     l.mediana_ms, c.mediana_ms, c.mediana_ms / l.mediana_ms,
                     iguais ? "iguais" : "DIFEREM");
        std::println("       (valor observado para impedir a eliminação: {})", obs % 1000);
    }
}
