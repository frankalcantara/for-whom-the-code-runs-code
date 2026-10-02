// Exercício 6.2, as músicas em órbita: a mediana inferior depois de cada aumento, com dois multiconjuntos,
// a metade de baixo e a de cima.
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0, m = 0;
    std::cin >> n >> m;
    std::vector<long long> c(n);
    for (auto& x : c) std::cin >> x;
    std::multiset<long long> baixo, alto;               // |baixo| = |alto| ou |alto| + 1, e baixo <= alto
    auto equilibra = [&] {
        if (baixo.size() > alto.size() + 1) { auto it = std::prev(baixo.end()); alto.insert(*it); baixo.erase(it); }
        else if (alto.size() > baixo.size()) { auto it = alto.begin(); baixo.insert(*it); alto.erase(it); }
    };
    auto insere = [&](long long x) {
        if (baixo.empty() || x <= *baixo.rbegin()) baixo.insert(x);
        else alto.insert(x);
        equilibra();
    };
    auto remove = [&](long long x) {                    // remove uma única ocorrência
        if (auto it = baixo.find(x); it != baixo.end()) baixo.erase(it);
        else alto.erase(alto.find(x));
        equilibra();
    };
    for (long long x : c) insere(x);
    std::string saida;
    for (int t = 0; t < m; ++t) {
        int i = 0;
        long long d = 0;
        std::cin >> i >> d;
        --i;
        remove(c[i]);
        c[i] += d;                                      // até 10^9 + 2 * 10^5 * 10^9, em long long
        insere(c[i]);
        saida += std::to_string(*baixo.rbegin());
        saida += '\n';
    }
    std::cout << saida;
}
