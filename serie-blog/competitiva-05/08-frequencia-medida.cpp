// Quatro soluções da análise de frequências, medidas no fluxo completo:
// moda, quantidade de valores únicos e tabela ordenada (resumida em uma soma de verificação).
#include <algorithm>
#include <print>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>
#include "medicao.hpp"

struct Resposta {
    int moda, freq_moda, unicos;
    long long verificacao;   // soma de (posição na tabela ordenada) * valor * frequência
    bool operator==(const Resposta&) const = default;
};

// Recebe a tabela (valor, frequência) já em ordem crescente de valor.
template <typename Tabela>
Resposta resumir(const Tabela& tabela) {
    Resposta r{0, 0, 0, 0};
    long long pos = 0;
    for (const auto& [valor, f] : tabela) {
        if (f > r.freq_moda) { r.moda = valor; r.freq_moda = f; }
        if (f == 1) ++r.unicos;
        r.verificacao += ++pos * valor * static_cast<long long>(f) % 1'000'003;
    }
    return r;
}

Resposta ingenua(const std::vector<int>& a) {
    std::vector<int> distintos;
    for (int x : a)
        if (std::ranges::find(distintos, x) == distintos.end()) distintos.push_back(x);
    std::ranges::sort(distintos);
    std::vector<std::pair<int, int>> tabela;
    for (int v : distintos) tabela.emplace_back(v, static_cast<int>(std::ranges::count(a, v)));
    return resumir(tabela);
}

Resposta por_ordenacao(std::vector<int> a) {   // cópia deliberada: a entrada fica intacta
    std::ranges::sort(a);
    std::vector<std::pair<int, int>> tabela;
    for (std::size_t i = 0; i < a.size();) {
        std::size_t j = i;
        while (j < a.size() && a[j] == a[i]) ++j;
        tabela.emplace_back(a[i], static_cast<int>(j - i));
        i = j;
    }
    return resumir(tabela);
}

Resposta por_hash(const std::vector<int>& a) {
    std::unordered_map<int, int> freq;
    freq.reserve(a.size());
    for (int x : a) ++freq[x];
    std::vector<std::pair<int, int>> tabela(freq.begin(), freq.end());
    std::ranges::sort(tabela);
    return resumir(tabela);
}

Resposta por_contagem(const std::vector<int>& a) {
    constexpr int V = 1'000'000;
    std::vector<int> freq(V + 1, 0);
    for (int x : a) ++freq[x];
    std::vector<std::pair<int, int>> tabela;
    for (int v = 1; v <= V; ++v)
        if (freq[v] > 0) tabela.emplace_back(v, freq[v]);
    return resumir(tabela);
}

int main() {
    std::mt19937 gerador(20260930);
    std::uniform_int_distribution<int> valor(1, 1'000'000);
    std::println("{:>9} | {:>10} | {:>10} | {:>10} | {:>10}", "n", "ingênua", "ordenação", "hash", "contagem");
    long long obs = 0;
    for (int n : {1'000, 10'000, 100'000, 1'000'000}) {
        std::vector<int> a(n);
        for (int& x : a) x = valor(gerador);
        const Resposta ref = por_ordenacao(a);
        bool ok = por_hash(a) == ref && por_contagem(a) == ref;
        double t_ing = -1;
        if (n <= 10'000) {
            ok = ok && ingenua(a) == ref;
            t_ing = medir([&] { return ingenua(a).verificacao; }, obs).mediana_ms;
        }
        const double t_ord = medir([&] { return por_ordenacao(a).verificacao; }, obs).mediana_ms;
        const double t_hash = medir([&] { return por_hash(a).verificacao; }, obs).mediana_ms;
        const double t_cont = medir([&] { return por_contagem(a).verificacao; }, obs).mediana_ms;
        if (t_ing < 0)
            std::println("{:>9} | {:>10} | {:>10.3f} | {:>10.3f} | {:>10.3f}", n, "omitida", t_ord, t_hash, t_cont);
        else
            std::println("{:>9} | {:>10.3f} | {:>10.3f} | {:>10.3f} | {:>10.3f}", n, t_ing, t_ord, t_hash, t_cont);
        std::println("respostas iguais para n = {}: {}", n, ok);
    }
    std::println("tempos em ms, mediana de 5 rodadas");
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
