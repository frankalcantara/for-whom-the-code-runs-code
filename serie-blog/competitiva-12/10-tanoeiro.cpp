// A2.3, o estoque do tanoeiro: o recurso mais escasso, medido em barris, decide.
#include <algorithm>
#include <iostream>
#include <print>

int main() {
    long long aduelas = 0, aros = 0, s = 0, h = 0;
    std::cin >> aduelas >> aros >> s >> h;
    const long long barris = std::min(aduelas / s, aros / h);
    // barris <= aduelas / s garante barris * s <= aduelas: os produtos não transbordam.
    std::println("{} {} {}", barris, aduelas - barris * s, aros - barris * h);
}
