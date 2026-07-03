#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long people;
    long long step;
    std::cin >> people >> step;

    long long survivor = 0;
    for (long long size = 2; size <= people; ++size) {
        survivor = (survivor + step) % size;
    }
    std::cout << survivor + 1 << '\n';
}
