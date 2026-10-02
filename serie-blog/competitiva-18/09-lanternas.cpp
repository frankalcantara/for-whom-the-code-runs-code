// Exercício 6.7, as lanternas do estacionamento: antecessor e sucessor de x entre as vagas ocupadas,
// com std::set e lower_bound.
#include <iostream>
#include <set>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q = 0;
    std::cin >> q;
    std::set<int> ocupadas;
    std::string saida;
    for (int t = 0; t < q; ++t) {
        char op = 0;
        int x = 0;
        std::cin >> op >> x;
        if (op == '+') ocupadas.insert(x);              // já ocupada: insert não faz nada
        else if (op == '-') ocupadas.erase(x);          // vazia: erase devolve 0 e não faz nada
        else {
            const auto it = ocupadas.lower_bound(x);    // primeira vaga >= x
            const int sucessor = it == ocupadas.end() ? -1 : *it;
            int antecessor = -1;
            if (it != ocupadas.end() && *it == x) antecessor = x;
            else if (it != ocupadas.begin()) antecessor = *std::prev(it);   // nunca decrementa begin()
            saida += std::to_string(antecessor) + ' ' + std::to_string(sucessor) + '\n';
        }
    }
    std::cout << saida;
}
