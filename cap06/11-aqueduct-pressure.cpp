#include <iostream>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(int n) : tree_(static_cast<std::size_t>(n + 1), 0) {}

    void add(int index, long long change) {
        for (int i = index; i < static_cast<int>(tree_.size());
             i += i & -i) {
            tree_[static_cast<std::size_t>(i)] += change;
        }
    }

    long long prefix_sum(int index) const {
        long long result = 0;
        for (int i = index; i > 0; i -= i & -i) {
            result += tree_[static_cast<std::size_t>(i)];
        }
        return result;
    }

private:
    std::vector<long long> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int stations = 0;
    int operations = 0;
    std::cin >> stations >> operations;

    FenwickTree difference(stations);
    while (operations-- > 0) {
        int type = 0;
        std::cin >> type;
        if (type == 1) {
            int left = 0;
            int right = 0;
            long long change = 0;
            std::cin >> left >> right >> change;
            difference.add(left, change);
            if (right < stations) {
                difference.add(right + 1, -change);
            }
        } else {
            int station = 0;
            std::cin >> station;
            std::cout << difference.prefix_sum(station) << '\n';
        }
    }
}
