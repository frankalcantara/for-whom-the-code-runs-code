#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr long long modulus = 1'000'000'007;
    int candidates = 0;
    int positions = 0;
    std::cin >> candidates >> positions;

    long long answer = 1;
    for (int offset = 0; offset < positions; ++offset) {
        answer =
            answer * static_cast<long long>(candidates - offset) % modulus;
    }
    std::cout << answer << '\n';
}
