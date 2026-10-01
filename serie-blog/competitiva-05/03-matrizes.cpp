// Três representações de uma matriz e as somas de prefixos em duas dimensões.
#include <mdspan>
#include <print>
#include <vector>
#include "medicao.hpp"

struct Grade {
    std::vector<int> dados;
    int colunas;
    int& operator[](int r, int c) { return dados[static_cast<std::size_t>(r) * colunas + c]; }
};

int main() {
#ifdef __cpp_multidimensional_subscript
    std::println("__cpp_multidimensional_subscript = {}", __cpp_multidimensional_subscript);
#else
    std::println("__cpp_multidimensional_subscript ausente");
#endif
#ifdef __cpp_lib_mdspan
    std::println("__cpp_lib_mdspan = {}", __cpp_lib_mdspan);
#else
    std::println("__cpp_lib_mdspan ausente");
#endif

    // 1. O endereço de a[r, c] em uma matriz plana de 3 x 4.
    std::vector<int> buffer(12);
    for (int i = 0; i < 12; ++i) buffer[i] = i;
    std::mdspan<int, std::dextents<std::size_t, 2>> m(buffer.data(), 3, 4);
    std::println("m[1, 2] = {}, m[2, 3] = {}, extents = {} x {}", m[1, 2], m[2, 3], m.extent(0), m.extent(1));
    Grade g{std::vector<int>(12, 0), 4};
    g[2, 1] = 42;
    std::println("g[2, 1] = 42 grava a posição {} do vetor: {}", 2 * 4 + 1, g.dados[9]);

    // 2. Somar uma matriz de 4000 x 4000 nas três representações.
    constexpr int L = 4000, C = 4000;
    std::vector<std::vector<int>> vv(L, std::vector<int>(C));
    std::vector<int> plano(static_cast<std::size_t>(L) * C);
    for (int r = 0; r < L; ++r)
        for (int c = 0; c < C; ++c) {
            const int x = (r * 31 + c * 17) % 1000;
            vv[r][c] = x;
            plano[static_cast<std::size_t>(r) * C + c] = x;
        }
    std::mdspan<int, std::dextents<std::size_t, 2>> vista(plano.data(), L, C);

    auto soma_vv = [&] {
        long long s = 0;
        for (int r = 0; r < L; ++r)
            for (int c = 0; c < C; ++c) s += vv[r][c];
        return s;
    };
    auto soma_plano = [&] {
        long long s = 0;
        for (int r = 0; r < L; ++r)
            for (int c = 0; c < C; ++c) s += plano[static_cast<std::size_t>(r) * C + c];
        return s;
    };
    auto soma_mdspan = [&] {
        long long s = 0;
        for (std::size_t r = 0; r < vista.extent(0); ++r)
            for (std::size_t c = 0; c < vista.extent(1); ++c) s += vista[r, c];
        return s;
    };
    auto soma_vv_colunas = [&] {
        long long s = 0;
        for (int c = 0; c < C; ++c)
            for (int r = 0; r < L; ++r) s += vv[r][c];
        return s;
    };
    long long obs = 0;
    const Medicao a = medir(soma_vv, obs);
    const Medicao b = medir(soma_plano, obs);
    const Medicao d = medir(soma_mdspan, obs);
    const Medicao e = medir(soma_vv_colunas, obs);
    std::println("somas iguais: {}", soma_vv() == soma_plano() && soma_plano() == soma_mdspan());
    std::println("vector<vector<int>>, por linhas : {:8.3f} ms", a.mediana_ms);
    std::println("vector<int> plano, por linhas   : {:8.3f} ms", b.mediana_ms);
    std::println("mdspan sobre o plano, por linhas: {:8.3f} ms", d.mediana_ms);
    std::println("vector<vector<int>>, por colunas: {:8.3f} ms", e.mediana_ms);

    // 3. Prefixos em duas dimensões para a matriz do exemplo do artigo.
    const std::vector<std::vector<int>> A{{3, 1, 4, 1}, {5, 9, 2, 6}, {5, 3, 5, 8}};
    const int R = 3, K = 4;
    std::vector<std::vector<long long>> P(R + 1, std::vector<long long>(K + 1, 0));
    for (int r = 1; r <= R; ++r)
        for (int c = 1; c <= K; ++c)
            P[r][c] = A[r - 1][c - 1] + P[r - 1][c] + P[r][c - 1] - P[r - 1][c - 1];
    for (int r = 0; r <= R; ++r) {
        std::print("P[{}]:", r);
        for (int c = 0; c <= K; ++c) std::print(" {}", P[r][c]);
        std::println("");
    }
    auto retangulo = [&](int r1, int c1, int r2, int c2) {
        return P[r2 + 1][c2 + 1] - P[r1][c2 + 1] - P[r2 + 1][c1] + P[r1][c1];
    };
    std::println("soma de (1,1) a (2,3) = {}", retangulo(1, 1, 2, 3));
    std::println("soma de (0,0) a (2,3) = {}", retangulo(0, 0, 2, 3));
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
