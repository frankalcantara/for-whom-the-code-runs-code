// A2.2, a cota do moinho: busca na resposta, forma de minimização, com teto sem overflow.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    long long horas = 0;
    std::cin >> n >> horas;
    std::vector<long long> monte(n);
    for (auto& h : monte) std::cin >> h;
    auto cabe = [&](long long r) {
        long long total = 0;
        for (const long long h : monte) {
            total += h / r + (h % r != 0 ? 1 : 0);    // teto de h / r sem calcular h + r - 1
            if (total > horas) return false;          // a saída antecipada também impede o overflow
        }
        return true;
    };
    long long lo = 1;
    long long hi = *std::ranges::max_element(monte);  // uma hora por monte, n <= H horas: cabe
    while (lo < hi) {
        const long long meio = lo + (hi - lo) / 2;
        if (cabe(meio)) hi = meio;
        else lo = meio + 1;
    }
    std::println("{}", lo);
}
