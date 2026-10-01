// Counting sort simples e estável contra std::ranges::sort, variando o universo k das chaves.
#include <algorithm>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

// Versão simples: conta as frequências e reescreve os valores. Não é estável.
void contagem_simples(std::vector<int>& a, int k, std::vector<int>& cont) {
    cont.assign(k, 0);
    for (int x : a) ++cont[x];
    std::size_t pos = 0;
    for (int valor = 0; valor < k; ++valor)
        for (int c = 0; c < cont[valor]; ++c) a[pos++] = valor;
}

// Versão estável: somas de prefixo e passada da direita para a esquerda.
void contagem_estavel(const std::vector<int>& a, int k, std::vector<int>& cont, std::vector<int>& saida) {
    cont.assign(k, 0);
    for (int x : a) ++cont[x];
    for (int i = 1; i < k; ++i) cont[i] += cont[i - 1];
    saida.resize(a.size());
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) saida[--cont[a[i]]] = a[i];
}

int main() {
    std::mt19937 gerador(20261001);
    long long obs = 0;
    std::vector<int> base, v, cont, saida, ref;

    auto tabela = [&](int n, const std::vector<int>& universos) {
        std::println("{:>10} | {:>10} | {:>12} | {:>12} | {:>12} | {}", "n", "k", "simples (ms)", "estável (ms)",
                     "sort (ms)", "iguais");
        for (int k : universos) {
            std::uniform_int_distribution<int> dist(0, k - 1);
            base.resize(n);
            for (int& x : base) x = dist(gerador);
            ref = base;
            std::ranges::sort(ref);
            auto copia = [&] { v = base; };
            const auto ms = medir_com_preparo(copia, [&] { contagem_simples(v, k, cont); return v[n / 2]; }, obs);
            const bool ok1 = v == ref;
            const auto me = medir_com_preparo(copia, [&] { contagem_estavel(v, k, cont, saida); return saida[n / 2]; }, obs);
            const bool ok2 = saida == ref;
            const auto mo = medir_com_preparo(copia, [&] { std::ranges::sort(v); return v[n / 2]; }, obs);
            std::println("{:>10} | {:>10} | {:>12.1f} | {:>12.1f} | {:>12.1f} | {}", n, k, ms.mediana_ms, me.mediana_ms,
                         mo.mediana_ms, ok1 && ok2);
        }
    };
    tabela(10'000'000, {1 << 8, 1 << 16, 1 << 20, 1 << 24});
    tabela(100'000, {100'000, 1'000'000, 10'000'000, 100'000'000});
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
