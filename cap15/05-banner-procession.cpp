#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int count;
    std::cin >> count;
    std::vector<std::string> banners(count);
    for (std::string& banner : banners) std::cin >> banner;

    std::ranges::sort(banners, [](const std::string& first,
                                  const std::string& second) {
        return first + second > second + first;
    });

    if (banners.front() == "0") {
        std::cout << "0\n";
        return 0;
    }
    for (const std::string& banner : banners) std::cout << banner;
    std::cout << '\n';
}
