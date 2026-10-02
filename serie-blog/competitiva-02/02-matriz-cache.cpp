// Multiplicação de matrizes em três ordens de laço: mesma conta, memória diferente.
#include <algorithm>
#include <print>
#include <vector>
#include "medicao.hpp"

using Matriz = std::vector<std::vector<int>>;

Matriz gerar(int n, int semente) {
    Matriz m(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            m[i][j] = (i * 31 + j * 17 + semente) % 7;  // valores pequenos: sem overflow
    return m;
}

// Ordem i-j-k: o laço interno avança k e percorre B por coluna.
Matriz mult_ijk(const Matriz& A, const Matriz& B) {
    const int n = static_cast<int>(A.size());
    Matriz C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Ordem i-k-j: o laço interno avança j e percorre B e C por linha.
Matriz mult_ikj(const Matriz& A, const Matriz& B) {
    const int n = static_cast<int>(A.size());
    Matriz C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            const int a = A[i][k];
            for (int j = 0; j < n; ++j)
                C[i][j] += a * B[k][j];
        }
    return C;
}

// Blocagem: a ordem i-k-j aplicada a blocos de BLOCO x BLOCO elementos.
constexpr int BLOCO = 64;
Matriz mult_blocos(const Matriz& A, const Matriz& B) {
    const int n = static_cast<int>(A.size());
    Matriz C(n, std::vector<int>(n, 0));
    for (int ii = 0; ii < n; ii += BLOCO)
        for (int kk = 0; kk < n; kk += BLOCO)
            for (int jj = 0; jj < n; jj += BLOCO)
                for (int i = ii; i < std::min(ii + BLOCO, n); ++i)
                    for (int k = kk; k < std::min(kk + BLOCO, n); ++k) {
                        const int a = A[i][k];
                        for (int j = jj; j < std::min(jj + BLOCO, n); ++j)
                            C[i][j] += a * B[k][j];
                    }
    return C;
}

long long assinatura(const Matriz& C) {
    long long s = 0;
    for (const auto& linha : C)
        for (int x : linha) s = s * 31 + x;
    return s;
}

int main() {
    // Verificação em uma matriz pequena: as três ordens produzem a mesma matriz.
    {
        const Matriz A = gerar(96, 1), B = gerar(96, 2);
        const bool iguais = mult_ijk(A, B) == mult_ikj(A, B) && mult_ikj(A, B) == mult_blocos(A, B);
        std::println("n = 96: as três ordens produzem a mesma matriz: {}", iguais);
    }
    const int n = 512;
    const Matriz A = gerar(n, 1), B = gerar(n, 2);
    long long obs = 0;
    const Medicao ijk = medir([&] { return assinatura(mult_ijk(A, B)); }, obs);
    const Medicao ikj = medir([&] { return assinatura(mult_ikj(A, B)); }, obs);
    const Medicao blo = medir([&] { return assinatura(mult_blocos(A, B)); }, obs);
    std::println("n = {}, 1 aquecimento e 5 rodadas por ordem", n);
    std::println("i-j-k : mediana {:8.1f} ms, mínimo {:8.1f} ms", ijk.mediana_ms, ijk.minimo_ms);
    std::println("i-k-j : mediana {:8.1f} ms, mínimo {:8.1f} ms", ikj.mediana_ms, ikj.minimo_ms);
    std::println("blocos: mediana {:8.1f} ms, mínimo {:8.1f} ms", blo.mediana_ms, blo.minimo_ms);
    std::println("razão i-j-k / i-k-j = {:.2f}", ijk.mediana_ms / ikj.mediana_ms);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
