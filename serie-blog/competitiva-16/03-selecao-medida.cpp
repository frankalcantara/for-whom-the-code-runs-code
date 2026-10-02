// Seleção da mediana com valores repetidos: partição de Lomuto em duas partes, partição em três
// partes e std::nth_element, em dados aleatórios, com 10 valores distintos e todos iguais.
#include <algorithm>
#include <print>
#include <random>
#include <utility>
#include <vector>
#include "medicao.hpp"

std::mt19937_64 sorteio(7);

int lomuto(std::vector<int>& a, int k) {
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (true) {
        if (lo == hi) return a[lo];
        std::swap(a[std::uniform_int_distribution<int>(lo, hi)(sorteio)], a[hi]);
        const int pivo = a[hi];
        int muro = lo;
        for (int i = lo; i < hi; ++i)
            if (a[i] <= pivo) std::swap(a[muro++], a[i]);  // iguais ao pivô vão todos para a esquerda
        std::swap(a[muro], a[hi]);
        if (k < muro) hi = muro - 1;
        else if (k > muro) lo = muro + 1;
        else return a[muro];
    }
}

int tres_partes(std::vector<int>& a, int k) {
    int lo = 0, hi = static_cast<int>(a.size()) - 1;
    while (true) {
        const int pivo = a[std::uniform_int_distribution<int>(lo, hi)(sorteio)];
        int menor = lo, i = lo, maior = hi;
        while (i <= maior) {
            if (a[i] < pivo) std::swap(a[menor++], a[i++]);
            else if (a[i] > pivo) std::swap(a[i], a[maior--]);
            else ++i;
        }
        if (k < menor) hi = menor - 1;
        else if (k > maior) lo = maior + 1;
        else return pivo;
    }
}

int main() {
    std::mt19937_64 gerador(20261016);
    long long obs = 0;
    for (const int n : {20'000, 1'000'000}) {
        struct Caso { const char* nome; int distintos; };
        for (const Caso c : {Caso{"aleatórios", 1'000'000'000}, Caso{"10 valores distintos", 10}, Caso{"todos iguais", 1}}) {
            std::vector<int> original(n), a;
            std::uniform_int_distribution<int> valor(0, c.distintos - 1);
            for (auto& x : original) x = valor(gerador);
            const int k = n / 2;
            auto prepara = [&] { a = original; };
            int r1 = -1, r2 = 0, r3 = 0;
            const bool roda_lomuto = n == 20'000 || c.distintos > 10;   // com n = 10^6 e repetidos, Lomuto levaria minutos
            Medicao m1{};
            if (roda_lomuto) m1 = medir_com_preparo(prepara, [&] { r1 = lomuto(a, k); return r1; }, obs);
            const auto m2 = medir_com_preparo(prepara, [&] { r2 = tres_partes(a, k); return r2; }, obs);
            const auto m3 = medir_com_preparo(prepara, [&] { std::ranges::nth_element(a, a.begin() + k); r3 = a[k]; return r3; }, obs);
            if (roda_lomuto)
                std::println("n = {}, {}: Lomuto {:.3f} ms, três partes {:.3f} ms, nth_element {:.3f} ms, mesma mediana: {}", n, c.nome,
                             m1.mediana_ms, m2.mediana_ms, m3.mediana_ms, r1 == r2 && r2 == r3);
            else
                std::println("n = {}, {}: Lomuto não medido, três partes {:.3f} ms, nth_element {:.3f} ms, mesma mediana: {}", n, c.nome,
                             m2.mediana_ms, m3.mediana_ms, r2 == r3);
        }
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
