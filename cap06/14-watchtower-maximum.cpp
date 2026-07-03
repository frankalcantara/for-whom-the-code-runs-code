#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

class MaximumTree {
public:
    explicit MaximumTree(const std::vector<long long>& values) {
        size_ = 1;
        while (size_ < static_cast<int>(values.size())) {
            size_ *= 2;
        }
        tree_.assign(
            static_cast<std::size_t>(2 * size_),
            std::numeric_limits<long long>::lowest());
        for (int i = 0; i < static_cast<int>(values.size()); ++i) {
            tree_[static_cast<std::size_t>(size_ + i)] =
                values[static_cast<std::size_t>(i)];
        }
        for (int node = size_ - 1; node > 0; --node) {
            tree_[static_cast<std::size_t>(node)] =
                std::max(tree_[static_cast<std::size_t>(2 * node)],
                         tree_[static_cast<std::size_t>(2 * node + 1)]);
        }
    }

    void assign(int position, long long value) {
        int node = size_ + position;
        tree_[static_cast<std::size_t>(node)] = value;
        for (node /= 2; node > 0; node /= 2) {
            tree_[static_cast<std::size_t>(node)] =
                std::max(tree_[static_cast<std::size_t>(2 * node)],
                         tree_[static_cast<std::size_t>(2 * node + 1)]);
        }
    }

    long long maximum(int left, int right) const {
        left += size_;
        right += size_;
        long long answer = std::numeric_limits<long long>::lowest();

        while (left <= right) {
            if ((left & 1) != 0) {
                answer =
                    std::max(answer, tree_[static_cast<std::size_t>(left++)]);
            }
            if ((right & 1) == 0) {
                answer =
                    std::max(answer, tree_[static_cast<std::size_t>(right--)]);
            }
            left /= 2;
            right /= 2;
        }
        return answer;
    }

private:
    int size_ = 0;
    std::vector<long long> tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int towers = 0;
    int operations = 0;
    std::cin >> towers >> operations;

    std::vector<long long> signal(static_cast<std::size_t>(towers));
    for (long long& value : signal) {
        std::cin >> value;
    }
    MaximumTree network(signal);

    while (operations-- > 0) {
        int type = 0;
        int first = 0;
        long long second = 0;
        std::cin >> type >> first >> second;
        if (type == 1) {
            network.assign(first - 1, second);
        } else {
            std::cout << network.maximum(first - 1,
                                         static_cast<int>(second) - 1)
                      << '\n';
        }
    }
}
