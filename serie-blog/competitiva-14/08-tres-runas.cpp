// Exercício 5.3, as três runas: trios de posições com soma zero, por ordenação e dois ponteiros.
#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n = 0;
    std::cin >> n;
    std::vector<long long> a(n);
    for (auto& x : a) std::cin >> x;
    std::ranges::sort(a);
    long long trios = 0;                              // até C(5000, 3), cerca de 2 * 10^10
    for (int i = 0; i + 2 < n; ++i) {
        int esq = i + 1, dir = n - 1;
        while (esq < dir) {
            const long long soma = a[i] + a[esq] + a[dir];
            if (soma < 0) ++esq;
            else if (soma > 0) --dir;
            else if (a[esq] == a[dir]) {              // todo o trecho [esq, dir] tem o mesmo valor
                const long long c = dir - esq + 1;
                trios += c * (c - 1) / 2;
                break;
            } else {
                long long ce = 1, cd = 1;
                while (esq + ce < dir && a[esq + ce] == a[esq]) ++ce;
                while (dir - cd > esq && a[dir - cd] == a[dir]) ++cd;
                trios += ce * cd;
                esq += static_cast<int>(ce);
                dir -= static_cast<int>(cd);
            }
        }
    }
    std::println("{}", trios);
}
