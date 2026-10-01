// T01.6, as duas balsas: intercala duas listas crescentes em uma só.
#include <cstddef>
#include <iostream>
#include <print>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<long long> primeira(n);
    for (auto& x : primeira) std::cin >> x;
    int m = 0;
    std::cin >> m;
    std::vector<long long> segunda(m);
    for (auto& x : segunda) std::cin >> x;

    std::vector<long long> registro;
    registro.reserve(static_cast<std::size_t>(n) + m);
    int i = 0, j = 0;
    // O menor horário ainda não registrado está sempre em uma das duas frentes.
    while (i < n && j < m)
        registro.push_back(primeira[i] < segunda[j] ? primeira[i++] : segunda[j++]);
    while (i < n) registro.push_back(primeira[i++]);
    while (j < m) registro.push_back(segunda[j++]);

    for (std::size_t k = 0; k < registro.size(); ++k)
        std::print("{}{}", registro[k], k + 1 == registro.size() ? "\n" : " ");
}
