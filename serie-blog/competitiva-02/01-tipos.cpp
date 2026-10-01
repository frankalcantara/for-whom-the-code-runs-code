// Tipos, faixas e conversões: os números da Seção 1 do Artigo 2.
#include <cstddef>
#include <limits>
#include <print>
#include <type_traits>
#include <vector>

int main() {
    std::println("sizeof(int) = {} bytes, sizeof(long long) = {} bytes",
                 sizeof(int), sizeof(long long));
    std::println("int vai de {} a {}",
                 std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    // n valores de até 10^9: a soma precisa de 64 bits.
    const int n = 200'000;
    const int valor = 1'000'000'000;
    long long total{0};
    for (int i = 0; i < n; ++i) total += valor;
    std::println("soma de {} valores iguais a {} = {}", n, valor, total);

    // A conversão precisa acontecer antes do produto, e não depois.
    const long long produto = static_cast<long long>(n) * valor;
    std::println("produto com conversão antes da multiplicação = {}", produto);

    // Aritmética sem sinal é definida módulo 2^w.
    unsigned x = 0;
    --x;
    std::println("unsigned 0 menos 1 = {}", x);

    // auto deduz o tipo a partir do inicializador.
    auto a = 1;
    auto b = 0LL;
    static_assert(std::is_same_v<decltype(a), int>);
    static_assert(std::is_same_v<decltype(b), long long>);
    std::println("auto a = 1 deduz int, auto b = 0LL deduz long long");

    // Tamanho sem sinal convertido explicitamente para um índice com sinal.
    const std::vector<int> v{3, 1, 4, 1, 5};
    int soma_v = 0;
    for (int i = 0; i < static_cast<int>(v.size()); ++i) soma_v += v[i];
    std::println("v.size() = {}, soma de v = {}", v.size(), soma_v);
}
