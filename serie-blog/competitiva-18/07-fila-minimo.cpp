// A fila do pedágio: uma fila com mínimo em O(1) amortizado, montada com duas pilhas que guardam,
// em cada nível, o valor e o mínimo até ali.
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct FilaMinimo {
    std::vector<std::pair<long long, long long>> entrada, saida;   // (valor, mínimo da pilha até aqui)
    static void empilha(std::vector<std::pair<long long, long long>>& p, long long x) {
        p.emplace_back(x, p.empty() ? x : std::min(x, p.back().second));
    }
    void push(long long x) { empilha(entrada, x); }
    void pop() {
        if (saida.empty())                                         // transfere invertendo a ordem
            while (!entrada.empty()) { empilha(saida, entrada.back().first); entrada.pop_back(); }
        saida.pop_back();
    }
    long long minimo() const {
        if (entrada.empty()) return saida.back().second;
        if (saida.empty()) return entrada.back().second;
        return std::min(entrada.back().second, saida.back().second);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q = 0;
    std::cin >> q;
    FilaMinimo fila;
    std::string saida;
    for (int t = 0; t < q; ++t) {
        char op = 0;
        std::cin >> op;
        if (op == '+') { long long x = 0; std::cin >> x; fila.push(x); }
        else if (op == '-') fila.pop();
        else { saida += std::to_string(fila.minimo()); saida += '\n'; }
    }
    std::cout << saida;
}
