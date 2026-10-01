// Comparadores, ordenação estrita fraca e capturas.
#include <algorithm>
#include <cstdlib>
#include <print>
#include <vector>

int main() {
    std::vector<int> v{3, -1, -3, 2, 1, -2};
    auto por_modulo = v;
    std::ranges::stable_sort(por_modulo, [](int a, int b) { return std::abs(a) < std::abs(b); });
    std::println("estável por |x|: {}", por_modulo);

    auto com_desempate = v;
    std::ranges::sort(com_desempate, [](int a, int b) {
        const int aa = std::abs(a), bb = std::abs(b);
        return aa == bb ? a < b : aa < bb;
    });
    std::println("|x| e depois x:  {}", com_desempate);

    long long base = 100;
    auto por_copia = [base](int x) { return x + base; };
    auto por_referencia = [&base](int x) { return x + base; };
    base = 500;
    std::println("por cópia: {}, por referência: {}", por_copia(1), por_referencia(1));
}
