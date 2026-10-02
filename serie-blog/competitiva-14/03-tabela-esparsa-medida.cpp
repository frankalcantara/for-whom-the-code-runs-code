// Mínimo de intervalo: varredura direta contra a sparse table, construção e consultas.
#include <algorithm>
#include <bit>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

struct TabelaEsparsa {
    std::vector<std::vector<int>> st;                 // st[k][i] = mínimo de a[i .. i + 2^k - 1]
    explicit TabelaEsparsa(const std::vector<int>& a) {
        const int n = static_cast<int>(a.size());
        const int niveis = std::bit_width(static_cast<unsigned>(n));
        st.assign(niveis, {});
        st[0] = a;
        for (int k = 1; k < niveis; ++k) {
            const int meio = 1 << (k - 1);
            st[k].resize(n - (1 << k) + 1);
            for (int i = 0; i + (1 << k) <= n; ++i) st[k][i] = std::min(st[k - 1][i], st[k - 1][i + meio]);
        }
    }
    int minimo(int l, int r) const {                  // intervalo fechado [l, r]
        const int k = std::bit_width(static_cast<unsigned>(r - l + 1)) - 1;
        return std::min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

int main() {
    std::mt19937 gerador(20261001);
    std::uniform_int_distribution<int> valor(-1'000'000'000, 1'000'000'000);
    long long obs = 0;
    auto nada = [] {};
    for (int n : {10'000, 1'000'000}) {
        std::vector<int> a(n);
        for (int& x : a) x = valor(gerador);
        std::uniform_int_distribution<int> pos(0, n - 1);
        const int q = 10'000'000;
        std::vector<std::pair<int, int>> iv(q);
        for (auto& [l, r] : iv) {
            l = pos(gerador); r = pos(gerador);
            if (l > r) std::swap(l, r);
        }
        TabelaEsparsa* t = nullptr;
        const auto construcao = medir_com_preparo(nada, [&] {
            delete t;
            t = new TabelaEsparsa(a);
            return t->st.back()[0];
        }, obs);
        long long celulas = 0;
        for (const auto& nivel : t->st) celulas += static_cast<long long>(nivel.size());
        const auto consultas = medir_com_preparo(nada, [&] {
            long long s = 0;
            for (auto [l, r] : iv) s += t->minimo(l, r);
            return s;
        }, obs);
        // Conferência e custo da varredura direta em 10^3 intervalos.
        bool iguais = true;
        long long parcelas = 0;
        const auto direta = medir_com_preparo<1>(nada, [&] {
            long long s = 0;
            for (int j = 0; j < 1'000; ++j) {
                auto [l, r] = iv[j];
                const int m = *std::min_element(a.begin() + l, a.begin() + r + 1);
                parcelas += r - l + 1;
                iguais = iguais && m == t->minimo(l, r);
                s += m;
            }
            return s;
        }, obs);
        std::println("n = {:>7}: {} níveis, {} células, construção {:.2f} ms, {:.2f} ns por consulta, "
                     "10^3 varreduras diretas {:.2f} ms, iguais: {}", n, t->st.size(), celulas, construcao.mediana_ms,
                     consultas.mediana_ms * 1e6 / q, direta.mediana_ms, iguais);
        delete t;
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
