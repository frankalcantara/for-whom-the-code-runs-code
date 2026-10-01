// Estatísticas de uma sequência lida com fread e um leitor de inteiros próprio.
#include <algorithm>
#include <climits>
#include <cstdio>
#include <print>

// Leitor com buffer: transfere blocos de 64 KiB com fread e analisa os bytes à mão.
class LeitorRapido {
    static constexpr std::size_t capacidade = 1u << 16;
    char buffer[capacidade]{};
    std::size_t pos = 0, len = 0;

    int proximo() {                                  // devolve o próximo byte ou -1 no fim da entrada
        if (pos == len) {
            len = std::fread(buffer, 1, capacidade, stdin);
            pos = 0;
            if (len == 0) return -1;
        }
        return static_cast<unsigned char>(buffer[pos++]);
    }

public:
    bool ler(long long& valor) {
        int c = proximo();
        while (c != -1 && c != '-' && (c < '0' || c > '9')) c = proximo();   // pula espaços e quebras
        if (c == -1) return false;
        const bool negativo = c == '-';
        if (negativo) c = proximo();
        long long v = 0;
        while (c >= '0' && c <= '9') {
            v = v * 10 + (c - '0');                  // valor = 10 * valor + algarismo
            c = proximo();
        }
        valor = negativo ? -v : v;
        return true;
    }
};

int main() {
    static LeitorRapido entrada;                     // 64 KiB fora da pilha
    long long n = 0;
    entrada.ler(n);
    long long soma = 0, minimo = LLONG_MAX, maximo = LLONG_MIN, positivos = 0;
    for (long long i = 0; i < n; ++i) {
        long long x = 0;
        entrada.ler(x);
        soma += x;
        minimo = std::min(minimo, x);
        maximo = std::max(maximo, x);
        if (x > 0) ++positivos;
    }
    std::println("{} {} {} {}", soma, minimo, maximo, positivos);
}
