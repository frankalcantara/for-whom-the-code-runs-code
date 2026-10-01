// std::span: uma vista sem posse sobre memória contígua.
#include <array>
#include <cstdint>
#include <print>
#include <span>
#include <vector>

long long soma(std::span<const int> s) {
    long long total = 0;
    for (int x : s) total += x;
    return total;
}

void soma_um(std::span<int> s) {
    for (int& x : s) ++x;
}

int main() {
    std::vector<int> v{7, 2, 9, 4, 1, 8};
    std::array<int, 3> a{10, 20, 30};
    int bruto[4] = {1, 1, 1, 1};
    std::println("sizeof(std::span<const int>) = {}", sizeof(std::span<const int>));
    std::println("sizeof(std::span<const int, 3>) = {}", sizeof(std::span<const int, 3>));
    std::println("soma(vetor) = {}, soma(array) = {}, soma(bruto) = {}", soma(v), soma(a), soma(bruto));

    auto meio = std::span<const int>(v).subspan(2, 3);
    std::println("subspan(2, 3) = {} {} {}, soma = {}", meio[0], meio[1], meio[2], soma(meio));
    std::println("subspan aponta para dentro de v: {}", meio.data() == v.data() + 2);

    soma_um(std::span<int>(v).first(2));
    std::println("depois de soma_um nos dois primeiros: {} {} {}", v[0], v[1], v[2]);

    // Uma vista criada antes de uma realocação continua apontando para o bloco antigo.
    v.shrink_to_fit();
    std::span<int> vista(v);
    const auto antes = reinterpret_cast<std::uintptr_t>(vista.data());
    v.push_back(42);
    const auto agora = reinterpret_cast<std::uintptr_t>(v.data());
    std::println("a vista ficou apontando para o bloco antigo: {}", antes != agora);
}
