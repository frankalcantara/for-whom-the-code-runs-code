// O livro no limite: leitura de valores entre LLONG_MIN e LLONG_MAX sem overflow.
#include <climits>
#include <cstdio>
#include <print>

class LeitorLimite {
    static constexpr std::size_t capacidade = 1u << 16;
    char buffer[capacidade]{};
    std::size_t pos = 0, len = 0;

    int proximo() {
        if (pos == len) {
            len = std::fread(buffer, 1, capacidade, stdin);
            pos = 0;
            if (len == 0) return -1;
        }
        return static_cast<unsigned char>(buffer[pos++]);
    }

public:
    long long ler() {
        int c = proximo();
        while (c != -1 && c != '-' && (c < '0' || c > '9')) c = proximo();
        const bool negativo = c == '-';
        if (negativo) c = proximo();
        unsigned long long magnitude = 0;            // |LLONG_MIN| = 2^63 cabe em unsigned long long
        while (c >= '0' && c <= '9') {
            magnitude = magnitude * 10 + static_cast<unsigned>(c - '0');
            c = proximo();
        }
        constexpr unsigned long long limite_negativo = static_cast<unsigned long long>(LLONG_MAX) + 1ull;
        if (!negativo) return static_cast<long long>(magnitude);
        if (magnitude == limite_negativo) return LLONG_MIN;   // o único negativo sem simétrico positivo
        return -static_cast<long long>(magnitude);
    }
};

int main() {
    static LeitorLimite entrada;
    const long long n = entrada.ler();
    long long minimo = LLONG_MAX, maximo = LLONG_MIN, negativos = 0;
    for (long long i = 0; i < n; ++i) {
        const long long v = entrada.ler();
        if (v < minimo) minimo = v;
        if (v > maximo) maximo = v;
        if (v < 0) ++negativos;
    }
    std::println("{} {} {}", minimo, maximo, negativos);
}
