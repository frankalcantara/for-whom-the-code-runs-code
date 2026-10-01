// Por que a busca do último verdadeiro precisa arredondar o meio para cima.
#include <print>

// Predicado de exemplo: verdadeiro para x <= 63, falso depois. O último verdadeiro é 63.
bool pred(long long x) { return x <= 63; }

long long ultimo_verdadeiro(long long lo, long long hi, bool arredonda_para_cima, int limite, int& passos) {
    passos = 0;
    while (lo < hi && passos < limite) {
        const long long meio = arredonda_para_cima ? lo + (hi - lo + 1) / 2 : lo + (hi - lo) / 2;
        ++passos;
        if (pred(meio)) lo = meio;
        else            hi = meio - 1;
    }
    return lo;
}

int main() {
    int passos = 0;
    const long long certo = ultimo_verdadeiro(0, 100, true, 1000, passos);
    std::println("arredondando para cima: resposta {} em {} passos", certo, passos);
    const long long errado = ultimo_verdadeiro(0, 100, false, 1000, passos);
    std::println("arredondando para baixo: parou em {} depois de {} passos, laço {}", errado, passos,
                 passos == 1000 ? "interrompido pelo limite" : "terminou");
}
