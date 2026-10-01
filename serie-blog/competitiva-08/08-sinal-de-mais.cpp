// Como std::from_chars trata os sinais: '-' é aceito, '+' não, e zeros à esquerda são aceitos.
#include <charconv>
#include <print>
#include <string_view>

int main() {
    for (std::string_view s : {"-0007", "+0042", "0042", "0000"}) {
        long long v = 999;
        const auto [p, erro] = std::from_chars(s.data(), s.data() + s.size(), v);
        std::println("{:>6}: consumidos={} erro={} valor={}", s, p - s.data(),
                     erro == std::errc{} ? "nenhum" : "invalid_argument", v);
    }
}
