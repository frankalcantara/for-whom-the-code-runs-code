#include <iostream>
#include <iterator>
#include <set>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int operations = 0;
    std::cin >> operations;

    std::set<int> occupied;
    while (operations-- > 0) {
        char type = '\0';
        int position = 0;
        std::cin >> type >> position;

        if (type == '+') {
            occupied.insert(position);
        } else if (type == '-') {
            occupied.erase(position);
        } else {
            const auto successor = occupied.lower_bound(position);

            int right = -1;
            if (successor != occupied.end()) {
                right = *successor;
            }

            int left = -1;
            if (successor != occupied.end() && *successor == position) {
                left = position;
            } else if (successor != occupied.begin()) {
                left = *std::prev(successor);
            }

            std::cout << left << ' ' << right << '\n';
        }
    }
}
