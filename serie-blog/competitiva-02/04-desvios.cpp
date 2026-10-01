// Previsão de desvios: o mesmo laço sobre dados embaralhados e ordenados.
#include <algorithm>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

// Corpo simples: o compilador pode trocar o desvio por uma seleção sem salto.
long long soma_simples(const std::vector<int>& a, int limiar) {
    long long s = 0;
    for (int x : a)
        if (x > limiar) s += x;
    return s;
}

// Corpo pesado: o trabalho do ramo tomado desencoraja a troca por seleção.
long long soma_com_desvio(const std::vector<int>& a, int limiar) {
    long long s = 0;
    for (int x : a)
        if (x > limiar) s += static_cast<long long>(x) * x + (x ^ 0x5bd1e995);
    return s;
}

// Sem desvio: calcula sempre a contribuição e a multiplica por 0 ou 1.
long long soma_sem_desvio(const std::vector<int>& a, int limiar) {
    long long s = 0;
    for (int x : a) {
        const long long contribuicao = static_cast<long long>(x) * x + (x ^ 0x5bd1e995);
        s += contribuicao * (x > limiar);
    }
    return s;
}

int main() {
    const int n = 20'000'000;
    const int limiar = 500'000'000;  // metade da faixa: o teste acerta perto de 50%
    std::mt19937 gerador(12345);
    std::uniform_int_distribution<int> dist(0, 1'000'000'000);
    std::vector<int> dados(n);
    for (int& x : dados) x = dist(gerador);

    long long obs = 0;
    const Medicao simples_emb = medir([&] { return soma_simples(dados, limiar); }, obs);
    const Medicao desvio_emb = medir([&] { return soma_com_desvio(dados, limiar); }, obs);
    const Medicao semdesv_emb = medir([&] { return soma_sem_desvio(dados, limiar); }, obs);
    const long long r_emb = soma_com_desvio(dados, limiar);

    std::ranges::sort(dados);
    const Medicao simples_ord = medir([&] { return soma_simples(dados, limiar); }, obs);
    const Medicao desvio_ord = medir([&] { return soma_com_desvio(dados, limiar); }, obs);
    const Medicao semdesv_ord = medir([&] { return soma_sem_desvio(dados, limiar); }, obs);
    const long long r_ord = soma_sem_desvio(dados, limiar);

    std::println("n = {}, 1 aquecimento e 5 rodadas, mediana em ms", n);
    std::println("corpo simples,  embaralhado: {:7.1f}", simples_emb.mediana_ms);
    std::println("corpo simples,  ordenado   : {:7.1f}", simples_ord.mediana_ms);
    std::println("com desvio,     embaralhado: {:7.1f}", desvio_emb.mediana_ms);
    std::println("com desvio,     ordenado   : {:7.1f}", desvio_ord.mediana_ms);
    std::println("sem desvio,     embaralhado: {:7.1f}", semdesv_emb.mediana_ms);
    std::println("sem desvio,     ordenado   : {:7.1f}", semdesv_ord.mediana_ms);
    std::println("as versões com e sem desvio concordam: {}", r_emb == r_ord);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
