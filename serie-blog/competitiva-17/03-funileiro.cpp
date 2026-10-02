// T05.3, a estrada do funileiro: o trecho contíguo e não vazio de maior lucro, por Kadane.
#include <iostream>
#include <print>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    long long atual = 0, melhor = 0;                    // somas até 10^15 em valor absoluto
    for (int i = 0; i < n; ++i) {
        long long p = 0;
        std::cin >> p;
        atual = (i == 0 || atual < 0) ? p : atual + p;  // recomeça quando o trecho anterior só atrapalha
        if (i == 0 || atual > melhor) melhor = atual;   // o melhor começa no primeiro vilarejo, nunca em 0
    }
    std::println("{}", melhor);
}
