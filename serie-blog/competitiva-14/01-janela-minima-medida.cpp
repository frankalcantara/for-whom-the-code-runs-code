// Mínimo de cada janela de tamanho k: varredura ingênua, std::deque e um deque de índices sobre um vetor.
#include <algorithm>
#include <deque>
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

// O(nk): cada janela examinada do zero.
void ingenua(const std::vector<int>& a, int k, std::vector<int>& saida) {
    const int n = static_cast<int>(a.size());
    saida.resize(n - k + 1);
    for (int i = 0; i + k <= n; ++i) saida[i] = *std::min_element(a.begin() + i, a.begin() + i + k);
}

// O(n) com std::deque de índices, valores crescentes da frente para o fundo.
void com_std_deque(const std::vector<int>& a, int k, std::vector<int>& saida) {
    const int n = static_cast<int>(a.size());
    saida.resize(n - k + 1);
    std::deque<int> dq;
    for (int i = 0; i < n; ++i) {
        if (!dq.empty() && dq.front() <= i - k) dq.pop_front();          // saiu da janela
        while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();       // nunca mais será o mínimo
        dq.push_back(i);
        if (i >= k - 1) saida[i - k + 1] = a[dq.front()];
    }
}

// O(n) com os índices em um vetor de n posições: a frente e o fundo são dois inteiros.
void com_vetor(const std::vector<int>& a, int k, std::vector<int>& saida, std::vector<int>& fila) {
    const int n = static_cast<int>(a.size());
    saida.resize(n - k + 1);
    fila.resize(n);
    int frente = 0, fundo = 0;                                         // a fila ocupa [frente, fundo)
    for (int i = 0; i < n; ++i) {
        if (frente < fundo && fila[frente] <= i - k) ++frente;
        while (frente < fundo && a[fila[fundo - 1]] >= a[i]) --fundo;
        fila[fundo++] = i;
        if (i >= k - 1) saida[i - k + 1] = a[fila[frente]];
    }
}

int main() {
    std::mt19937 gerador(20261001);
    std::uniform_int_distribution<int> valor(-1'000'000'000, 1'000'000'000);
    const int n = 1'000'000;
    std::vector<int> a(n);
    for (int& x : a) x = valor(gerador);
    std::vector<int> s1, s2, s3, fila;
    long long obs = 0;
    auto nada = [] {};
    for (int k : {10, 1'000, 100'000}) {
        Medicao t1{};
        bool comparar = k <= 1'000;                                      // a ingênua com k = 10^5 faria 10^11 passos
        if (comparar) t1 = medir_com_preparo<1>(nada, [&] { ingenua(a, k, s1); return s1[0]; }, obs);
        const auto t2 = medir_com_preparo(nada, [&] { com_std_deque(a, k, s2); return s2[0]; }, obs);
        const auto t3 = medir_com_preparo(nada, [&] { com_vetor(a, k, s3, fila); return s3[0]; }, obs);
        const bool iguais = s2 == s3 && (!comparar || s1 == s2);
        if (comparar)
            std::println("n = 10^6, k = {:>6}: ingênua {:.1f} ms, std::deque {:.2f} ms, vetor {:.2f} ms, iguais: {}", k,
                         t1.mediana_ms, t2.mediana_ms, t3.mediana_ms, iguais);
        else
            std::println("n = 10^6, k = {:>6}: ingênua não medida, std::deque {:.2f} ms, vetor {:.2f} ms, iguais: {}", k,
                         t2.mediana_ms, t3.mediana_ms, iguais);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
