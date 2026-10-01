// T01.2, a conta do mestre dos fornos: soma de a até b por uma fórmula fechada.
#include <iostream>
#include <print>

int main() {
    long long a = 0, b = 0;
    std::cin >> a >> b;

    const long long extremos = a + b;        // até 4 * 10^9: já não cabe em int
    const long long quantidade = b - a + 1;  // até 2 * 10^9
    // Um dos dois fatores é par. Dividir esse fator por 2 antes do produto
    // mantém o resultado abaixo de 2,1 * 10^18.
    const long long total = (extremos % 2 == 0) ? (extremos / 2) * quantidade
                                                : extremos * (quantidade / 2);
    std::println("{}", total);
}
