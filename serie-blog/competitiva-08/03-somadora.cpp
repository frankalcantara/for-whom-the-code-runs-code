// T03.3, a máquina de somar antiga: sinal opcional e zeros à esquerda.
#include <charconv>
#include <iostream>
#include <print>
#include <string>
#include <string_view>

// Converte um token com sinal opcional. std::from_chars aceita '-', mas não aceita '+'.
long long converter(std::string_view token) {
    bool negativo = false;
    if (token.front() == '+' || token.front() == '-') {
        negativo = token.front() == '-';
        token.remove_prefix(1);
    }
    long long magnitude = 0;                         // zeros à esquerda não mudam 10 * v + d a partir de 0
    std::from_chars(token.data(), token.data() + token.size(), magnitude);
    return negativo ? -magnitude : magnitude;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    long long total = 0;
    std::string token;
    for (int i = 0; i < n; ++i) {
        std::cin >> token;
        total += converter(token);
    }
    std::println("{}", total);
}
