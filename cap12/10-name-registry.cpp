#include <iostream>
#include <string>
#include <unordered_set>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;

    std::unordered_set<std::string> names;
    names.reserve(static_cast<std::size_t>(count) * 2);
    names.max_load_factor(0.7F);

    while (count-- > 0) {
        std::string name;
        std::cin >> name;
        names.insert(std::move(name));
    }

    std::cout << names.size() << '\n';
}
