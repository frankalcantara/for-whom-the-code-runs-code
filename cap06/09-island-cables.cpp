#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(int n)
        : parent_(static_cast<std::size_t>(n)),
          size_(static_cast<std::size_t>(n), 1),
          components_(n) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int x) {
        while (parent_[static_cast<std::size_t>(x)] != x) {
            parent_[static_cast<std::size_t>(x)] =
                parent_[static_cast<std::size_t>(
                    parent_[static_cast<std::size_t>(x)])];
            x = parent_[static_cast<std::size_t>(x)];
        }
        return x;
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (size_[static_cast<std::size_t>(a)]
            < size_[static_cast<std::size_t>(b)]) {
            std::swap(a, b);
        }
        parent_[static_cast<std::size_t>(b)] = a;
        size_[static_cast<std::size_t>(a)] +=
            size_[static_cast<std::size_t>(b)];
        --components_;
        return true;
    }

    int components() const {
        return components_;
    }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
    int components_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int islands = 0;
    int cables = 0;
    std::cin >> islands >> cables;

    DisjointSet groups(islands);
    while (cables-- > 0) {
        int a = 0;
        int b = 0;
        std::cin >> a >> b;
        groups.unite(a - 1, b - 1);
        std::cout << groups.components() << '\n';
    }
}
