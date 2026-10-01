// T03.6, a folha de entrada do celeiro: validação em ordem, com a primeira falha como resposta.
#include <expected>
#include <iostream>
#include <print>
#include <string_view>
#include <unordered_set>

std::expected<long long, std::string_view> auditar(std::istream& in) {
    long long n = 0;
    if (!(in >> n)) return std::unexpected("truncated");
    std::unordered_set<long long> vistos;           // números de produtor até 10^9: conjunto, e não vetor
    vistos.reserve(static_cast<std::size_t>(n));
    long long total = 0;                             // até 10^5 * 10^9 = 10^14
    for (long long i = 0; i < n; ++i) {
        long long produtor = 0, sacos = 0;
        if (!(in >> produtor >> sacos)) return std::unexpected("truncated");
        if (sacos <= 0) return std::unexpected("bad count");
        if (!vistos.insert(produtor).second) return std::unexpected("duplicate farmer");
        total += sacos;
    }
    return total;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    if (const auto veredito = auditar(std::cin)) std::println("ACCEPTED {}", *veredito);
    else                                        std::println("REJECTED {}", veredito.error());
}
