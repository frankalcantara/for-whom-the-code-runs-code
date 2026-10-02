// Tamanho da interseção de dois conjuntos de um universo de 4096 elementos em cinco representações.
#include <bit>
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#if defined(_MSC_VER)
#include <intrin.h>
#endif
#include "medicao.hpp"

constexpr std::size_t U = 4096, N = 2000, W = U / 64;
using Conjunto = std::bitset<U>;

int main() {
    std::mt19937_64 gerador(20261015);
    std::vector<std::uint8_t> bytes(N * U);                    // um byte por elemento, 0 ou 1
    std::vector<bool> compactos(N * U);                        // um bit por elemento, acesso por proxy
    std::vector<std::uint64_t> palavras(N * W);                // 64 elementos por palavra
    std::vector<Conjunto> bitsets(N);
    for (std::size_t r = 0; r < N; ++r)
        for (std::size_t j = 0; j < W; ++j) {
            const std::uint64_t p = gerador();                 // densidade 1/2
            palavras[r * W + j] = p;
            for (std::size_t b = 0; b < 64; ++b) {
                const bool ligado = (p >> b) & 1;
                bytes[r * U + j * 64 + b] = ligado;
                compactos[r * U + j * 64 + b] = ligado;
                bitsets[r][j * 64 + b] = ligado;
            }
        }
    const std::size_t Q = 200'000, Q_bool = 20'000;
    std::vector<std::size_t> pa(Q), pb(Q);
    std::uniform_int_distribution<std::size_t> qual(0, N - 1);
    for (std::size_t q = 0; q < Q; ++q) { pa[q] = qual(gerador); pb[q] = qual(gerador); }

    auto por_bytes = [&](std::size_t quantas) {
        std::size_t total = 0;
        for (std::size_t q = 0; q < quantas; ++q) {
            const std::uint8_t* x = bytes.data() + pa[q] * U;
            const std::uint8_t* y = bytes.data() + pb[q] * U;
            std::size_t c = 0;
            for (std::size_t i = 0; i < U; ++i) c += x[i] & y[i];
            total += c;
        }
        return total;
    };
    auto por_vector_bool = [&](std::size_t quantas) {
        std::size_t total = 0;
        for (std::size_t q = 0; q < quantas; ++q) {
            const std::size_t x = pa[q] * U, y = pb[q] * U;
            for (std::size_t i = 0; i < U; ++i) total += (compactos[x + i] && compactos[y + i]) ? 1 : 0;
        }
        return total;
    };
    auto por_popcount = [&](std::size_t quantas) {
        std::size_t total = 0;
        for (std::size_t q = 0; q < quantas; ++q) {
            const std::uint64_t* x = palavras.data() + pa[q] * W;
            const std::uint64_t* y = palavras.data() + pb[q] * W;
            for (std::size_t j = 0; j < W; ++j) total += static_cast<std::size_t>(std::popcount(x[j] & y[j]));
        }
        return total;
    };
    auto por_intrinseca = [&](std::size_t quantas) {
        std::size_t total = 0;
        for (std::size_t q = 0; q < quantas; ++q) {
            const std::uint64_t* x = palavras.data() + pa[q] * W;
            const std::uint64_t* y = palavras.data() + pb[q] * W;
#if defined(_MSC_VER)
            for (std::size_t j = 0; j < W; ++j) total += __popcnt64(x[j] & y[j]);   // instrução popcnt, sem teste
#else
            for (std::size_t j = 0; j < W; ++j) total += static_cast<std::size_t>(std::popcount(x[j] & y[j]));
#endif
        }
        return total;
    };
    auto por_bitset = [&](std::size_t quantas) {
        std::size_t total = 0;
        for (std::size_t q = 0; q < quantas; ++q) total += (bitsets[pa[q]] & bitsets[pb[q]]).count();
        return total;
    };

    long long obs = 0;
    auto nada = [] {};
    std::size_t referencia = 0, referencia_bool = 0;
    struct Linha { const char* nome; std::size_t quantas; Medicao m; std::size_t r; };
    std::vector<Linha> linhas;
    std::size_t r = 0;
    auto m1 = medir_com_preparo(nada, [&] { r = por_bytes(Q); return r; }, obs);       linhas.push_back({"bytes", Q, m1, r});
    referencia = r;
    auto m2 = medir_com_preparo(nada, [&] { r = por_vector_bool(Q_bool); return r; }, obs); linhas.push_back({"std::vector<bool>", Q_bool, m2, r});
    referencia_bool = por_bytes(Q_bool);
    auto m3 = medir_com_preparo(nada, [&] { r = por_popcount(Q); return r; }, obs);    linhas.push_back({"palavras com std::popcount", Q, m3, r});
    auto m4 = medir_com_preparo(nada, [&] { r = por_intrinseca(Q); return r; }, obs);  linhas.push_back({"palavras com __popcnt64", Q, m4, r});
    auto m5 = medir_com_preparo(nada, [&] { r = por_bitset(Q); return r; }, obs);      linhas.push_back({"std::bitset<4096>", Q, m5, r});
    for (const auto& l : linhas) {
        const bool ok = l.r == (l.quantas == Q ? referencia : referencia_bool);
        std::println("{}: {:.2f} ms em {} consultas, {:.1f} ns por consulta, confere: {}", l.nome, l.m.mediana_ms, l.quantas,
                     l.m.mediana_ms * 1e6 / static_cast<double>(l.quantas), ok);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
