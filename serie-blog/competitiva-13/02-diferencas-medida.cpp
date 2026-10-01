// Atualizações de intervalo: aplicar diretamente contra marcar as fronteiras em um array de diferenças.
#include <print>
#include <random>
#include <vector>
#include "medicao.hpp"

struct Operacao { int l, r; long long x; };

int main() {
    std::mt19937 gerador(20261001);
    long long obs = 0;
    for (int n : {20'000, 200'000}) {
        const int m = n;
        std::uniform_int_distribution<int> pos(0, n - 1);
        std::uniform_int_distribution<int> val(-1'000'000'000, 1'000'000'000);
        std::vector<Operacao> ops(m);
        long long celulas = 0;
        for (auto& o : ops) {
            o.l = pos(gerador); o.r = pos(gerador); o.x = val(gerador);
            if (o.l > o.r) std::swap(o.l, o.r);
            celulas += o.r - o.l + 1;
        }
        std::vector<long long> direto(n), via(n), d(n + 1);
        const auto td = medir_com_preparo<1>([&] { std::ranges::fill(direto, 0); }, [&] {
            for (const auto& o : ops) for (int i = o.l; i <= o.r; ++i) direto[i] += o.x;
            return direto[n / 2];
        }, obs);
        const auto tf = medir_com_preparo([&] { std::ranges::fill(d, 0); }, [&] {
            for (const auto& o : ops) { d[o.l] += o.x; d[o.r + 1] -= o.x; }
            long long corrente = 0;
            for (int i = 0; i < n; ++i) { corrente += d[i]; via[i] = corrente; }
            return via[n / 2];
        }, obs);
        std::println("n = m = {}: direto {:.2f} ms (1 rodada) ({} somas), diferenças {:.3f} ms, iguais: {}", n, td.mediana_ms,
                     celulas, tf.mediana_ms, direto == via);
    }
    std::println("valor observado para impedir a eliminação: {}", obs % 1000);
}
