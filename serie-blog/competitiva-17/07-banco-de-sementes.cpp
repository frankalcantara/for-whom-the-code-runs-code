// T05.7, o banco de sementes: quantas variedades distintas há em cada intervalo de prateleiras, pelo
// algoritmo de Mo, com blocos de raiz de n, ordem em serpentina e os códigos comprimidos.
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

struct Consulta { int l, r, id; };

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, q = 0;
    std::cin >> n >> q;
    std::vector<int> a(n);
    for (auto& x : a) std::cin >> x;
    std::vector<int> valores = a;                              // compressão: valores até 10^9 viram 0..d-1
    std::ranges::sort(valores);
    valores.erase(std::unique(valores.begin(), valores.end()), valores.end());
    for (auto& x : a) x = static_cast<int>(std::ranges::lower_bound(valores, x) - valores.begin());
    std::vector<Consulta> qs(q);
    for (int i = 0; i < q; ++i) {
        std::cin >> qs[i].l >> qs[i].r;
        --qs[i].l; --qs[i].r;                                  // para índices a partir de 0
        qs[i].id = i;
    }
    const int bloco = std::max(1, static_cast<int>(std::sqrt(static_cast<double>(n))));
    std::ranges::sort(qs, [bloco](const Consulta& x, const Consulta& y) {
        const int bx = x.l / bloco, by = y.l / bloco;
        if (bx != by) return bx < by;
        return (bx & 1) ? (x.r > y.r) : (x.r < y.r);           // serpentina
    });
    std::vector<int> cont(valores.size(), 0), resposta(q);
    int distintos = 0, L = 0, R = -1;                          // janela vazia [0, -1]
    auto entra = [&](int i) { if (cont[a[i]]++ == 0) ++distintos; };
    auto sai = [&](int i) { if (--cont[a[i]] == 0) --distintos; };
    for (const auto& c : qs) {
        while (R < c.r) entra(++R);                            // primeiro expandir
        while (L > c.l) entra(--L);
        while (R > c.r) sai(R--);                              // depois encolher
        while (L < c.l) sai(L++);
        resposta[c.id] = distintos;
    }
    std::string saida;
    for (int x : resposta) { saida += std::to_string(x); saida += '\n'; }
    std::cout << saida;
}
