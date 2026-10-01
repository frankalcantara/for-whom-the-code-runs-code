// T01.3, as provas da forja: conta as medições até 10% acima da mais rápida.
#include <iostream>
#include <print>
#include <vector>

int main() {
    int r = 0;
    std::cin >> r;
    std::vector<long long> t(r);
    for (auto& x : t) std::cin >> x;

    // A medição t[0] é o aquecimento e fica de fora das duas passadas.
    long long mais_rapida = t[1];
    for (int i = 2; i < r; ++i)
        if (t[i] < mais_rapida) mais_rapida = t[i];

    // t <= 1,1 m equivale a 10 t <= 11 m, uma comparação exata entre inteiros.
    int fieis = 0;
    for (int i = 1; i < r; ++i)
        if (10 * t[i] <= 11 * mais_rapida) ++fieis;
    std::println("{}", fieis);
}
