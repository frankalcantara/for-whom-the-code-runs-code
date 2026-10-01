// Vetor de registros contra registro de vetores.
#include <print>
#include <vector>
#include "medicao.hpp"

struct Ponto {
    int x, y, w;
};

struct Pontos {
    std::vector<int> x, y, w;
};

int main() {
    constexpr int N = 30'000'000;
    std::println("sizeof(Ponto) = {} bytes", sizeof(Ponto));
    long long obs = 0;
    Medicao aos_x{}, aos_tudo{}, soa_x{}, soa_tudo{};
    long long r1 = 0, r2 = 0, r3 = 0, r4 = 0;
    {
        std::vector<Ponto> pts(N);
        for (int i = 0; i < N; ++i) pts[i] = {i % 1000, (i * 7) % 1000, (i * 13) % 1000};
        auto so_x = [&] { long long s = 0; for (const auto& p : pts) s += p.x; return s; };
        auto tudo = [&] { long long s = 0; for (const auto& p : pts) s += p.x + p.y + p.w; return s; };
        aos_x = medir(so_x, obs);
        aos_tudo = medir(tudo, obs);
        r1 = so_x();
        r2 = tudo();
    }
    {
        Pontos soa;
        soa.x.resize(N); soa.y.resize(N); soa.w.resize(N);
        for (int i = 0; i < N; ++i) {
            soa.x[i] = i % 1000; soa.y[i] = (i * 7) % 1000; soa.w[i] = (i * 13) % 1000;
        }
        auto so_x = [&] { long long s = 0; for (int v : soa.x) s += v; return s; };
        auto tudo = [&] {
            long long s = 0;
            for (std::size_t i = 0; i < soa.x.size(); ++i) s += soa.x[i] + soa.y[i] + soa.w[i];
            return s;
        };
        soa_x = medir(so_x, obs);
        soa_tudo = medir(tudo, obs);
        r3 = so_x();
        r4 = tudo();
    }
    std::println("resultados iguais: {}", r1 == r3 && r2 == r4);
    std::println("3 * 10^7 registros, mediana de 5 rodadas");
    std::println("AoS, só o campo x   : {:8.3f} ms", aos_x.mediana_ms);
    std::println("SoA, só o campo x   : {:8.3f} ms", soa_x.mediana_ms);
    std::println("AoS, os três campos : {:8.3f} ms", aos_tudo.mediana_ms);
    std::println("SoA, os três campos : {:8.3f} ms", soa_tudo.mediana_ms);
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
