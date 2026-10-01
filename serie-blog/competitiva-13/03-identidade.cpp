// Prefixos e diferenças são operações inversas: rastreamentos pequenos em uma e em duas dimensões.
#include <print>
#include <vector>

void mostrar(const char* nome, const std::vector<long long>& v) {
    std::print("{}:", nome);
    for (long long x : v) std::print(" {:>3}", x);
    std::println();
}

int main() {
    const std::vector<long long> a{3, 1, 4, 1, 5, 9, 2};
    const int n = static_cast<int>(a.size());
    std::vector<long long> P(n + 1, 0), D(n), aP(n), aD(n);
    for (int i = 0; i < n; ++i) P[i + 1] = P[i] + a[i];
    for (int i = 0; i < n; ++i) D[i] = a[i] - (i > 0 ? a[i - 1] : 0);
    for (int i = 0; i < n; ++i) aP[i] = P[i + 1] - P[i];             // diferença dos prefixos
    long long c = 0;
    for (int i = 0; i < n; ++i) { c += D[i]; aD[i] = c; }             // prefixos das diferenças
    mostrar("a", a);
    mostrar("P, prefixos de a (P[0] = 0)", P);
    mostrar("D, diferenças de a", D);
    mostrar("diferenças de P", aP);
    mostrar("prefixos de D", aD);
    std::println("soma de a[2..5], fechado: P[6] - P[2] = {} - {} = {}", P[6], P[2], P[6] - P[2]);

    // Duas atualizações de intervalo marcadas nas fronteiras.
    std::vector<long long> d(n + 1, 0);
    auto soma_intervalo = [&](int l, int r, long long x) { d[l] += x; d[r + 1] -= x; };
    soma_intervalo(1, 4, 5);
    soma_intervalo(3, 6, -2);
    mostrar("marcas: +5 em [1, 4] e -2 em [3, 6]", d);
    std::vector<long long> final(n);
    c = 0;
    for (int i = 0; i < n; ++i) { c += d[i]; final[i] = c; }
    mostrar("efeito, prefixos das marcas", final);

    // Diferenças em duas dimensões: +4 no retângulo [1..2] x [1..3] de uma grade 4 x 5.
    const int R = 4, C = 5;
    std::vector<std::vector<long long>> D2(R + 1, std::vector<long long>(C + 1, 0));
    auto retangulo = [&](int r1, int c1, int r2, int c2, long long x) {
        D2[r1][c1] += x; D2[r1][c2 + 1] -= x; D2[r2 + 1][c1] -= x; D2[r2 + 1][c2 + 1] += x;
    };
    retangulo(1, 1, 2, 3, 4);
    retangulo(0, 2, 3, 2, 1);
    std::println("marcas em duas dimensões:");
    for (int r = 0; r <= R; ++r) {
        for (int col = 0; col <= C; ++col) std::print(" {:>3}", D2[r][col]);
        std::println();
    }
    for (int r = 0; r < R; ++r)                                        // prefixos por linha, depois por coluna
        for (int col = 1; col < C; ++col) D2[r][col] += D2[r][col - 1];
    for (int r = 1; r < R; ++r)
        for (int col = 0; col < C; ++col) D2[r][col] += D2[r - 1][col];
    std::println("grade final:");
    for (int r = 0; r < R; ++r) {
        for (int col = 0; col < C; ++col) std::print(" {:>3}", D2[r][col]);
        std::println();
    }
}
