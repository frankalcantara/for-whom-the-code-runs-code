// O algoritmo de Mo em 2 * 10^5 consultas de valores distintos: ordem simples contra serpentina e três
// tamanhos de bloco, com a contagem dos passos dos ponteiros.
#include <algorithm>
#include <cmath>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

struct Consulta { int l, r, id; };

int main() {
    std::mt19937_64 gerador(20261016);
    const int n = 200'000, q = 200'000;
    std::vector<int> a(n);
    std::uniform_int_distribution<int> valor(0, n - 1);
    for (auto& x : a) x = valor(gerador);
    std::vector<Consulta> base(q);
    std::uniform_int_distribution<int> pos(0, n - 1);
    for (int i = 0; i < q; ++i) { int l = pos(gerador), r = pos(gerador); if (l > r) std::swap(l, r); base[i] = {l, r, i}; }
    long long obs = 0;
    std::vector<int> referencia;
    auto roda = [&](int bloco, bool serpentina, long long& passos) {
        std::vector<Consulta> qs = base;
        std::ranges::sort(qs, [=](const Consulta& x, const Consulta& y) {
            const int bx = x.l / bloco, by = y.l / bloco;
            if (bx != by) return bx < by;
            return (serpentina && (bx & 1)) ? (x.r > y.r) : (x.r < y.r);
        });
        std::vector<int> cont(n, 0), resp(q);
        int distintos = 0, L = 0, R = -1;
        passos = 0;
        for (const auto& c : qs) {
            while (R < c.r) { if (cont[a[++R]]++ == 0) ++distintos; ++passos; }
            while (L > c.l) { if (cont[a[--L]]++ == 0) ++distintos; ++passos; }
            while (R > c.r) { if (--cont[a[R--]] == 0) --distintos; ++passos; }
            while (L < c.l) { if (--cont[a[L++]] == 0) --distintos; ++passos; }
            resp[c.id] = distintos;
        }
        if (referencia.empty()) referencia = resp;
        long long soma = 0;
        for (int x : resp) soma += x;
        return resp == referencia ? soma : -1;
    };
    auto nada = [] {};
    const int raiz = static_cast<int>(std::sqrt(static_cast<double>(n)));
    struct Caso { int bloco; bool serpentina; };
    for (const Caso c : {Caso{raiz, false}, Caso{raiz, true}, Caso{100, true}, Caso{2000, true}}) {
        long long passos = 0, r = 0;
        const auto m = medir_com_preparo<3>(nada, [&] { r = roda(c.bloco, c.serpentina, passos); return r; }, obs);
        std::println("bloco {}, {}: {:.1f} ms, {} passos dos ponteiros, respostas conferem: {}", c.bloco,
                     c.serpentina ? "serpentina" : "ordem simples", m.mediana_ms, passos, r >= 0);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
