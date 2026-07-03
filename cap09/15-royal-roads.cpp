#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <utility>
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
            tree_[static_cast<std::size_t>(size_ + i)] = values[i];
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

    [[nodiscard]] long long maximum(int left, int right) const {
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

class HeavyLight {
public:
    HeavyLight(const std::vector<std::vector<int>>& graph,
               const std::vector<long long>& values)
        : parent_(graph.size(), -1),
          depth_(graph.size(), 0),
          subtree_size_(graph.size(), 1),
          heavy_child_(graph.size(), -1),
          head_(graph.size(), 0),
          position_(graph.size(), 0),
          tree_(build(graph, values)) {}

    void assign(int vertex, long long value) {
        tree_.assign(position_[vertex], value);
    }

    [[nodiscard]] long long path_maximum(int first, int second) const {
        long long answer = std::numeric_limits<long long>::lowest();

        while (head_[first] != head_[second]) {
            if (depth_[head_[first]] < depth_[head_[second]]) {
                std::swap(first, second);
            }

            answer = std::max(
                answer,
                tree_.maximum(position_[head_[first]], position_[first]));
            first = parent_[head_[first]];
        }

        if (position_[first] > position_[second]) {
            std::swap(first, second);
        }
        return std::max(
            answer, tree_.maximum(position_[first], position_[second]));
    }

private:
    std::vector<long long> build(
        const std::vector<std::vector<int>>& graph,
        const std::vector<long long>& values) {
        std::vector<int> order;
        order.reserve(graph.size());
        std::vector<int> stack{0};

        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            order.push_back(vertex);

            for (int next : graph[vertex]) {
                if (next == parent_[vertex]) {
                    continue;
                }
                parent_[next] = vertex;
                depth_[next] = depth_[vertex] + 1;
                stack.push_back(next);
            }
        }

        for (auto it = order.rbegin(); it != order.rend(); ++it) {
            const int vertex = *it;
            int largest_child_size = 0;

            for (int next : graph[vertex]) {
                if (parent_[next] != vertex) {
                    continue;
                }
                subtree_size_[vertex] += subtree_size_[next];
                if (subtree_size_[next] > largest_child_size) {
                    largest_child_size = subtree_size_[next];
                    heavy_child_[vertex] = next;
                }
            }
        }

        std::vector<long long> linear(graph.size());
        std::vector<std::pair<int, int>> pending{{0, 0}};
        int timer = 0;

        while (!pending.empty()) {
            const auto [start, chain_head] = pending.back();
            pending.pop_back();

            for (int vertex = start; vertex != -1;
                 vertex = heavy_child_[vertex]) {
                head_[vertex] = chain_head;
                position_[vertex] = timer;
                linear[static_cast<std::size_t>(timer)] = values[vertex];
                ++timer;

                for (int next : graph[vertex]) {
                    if (parent_[next] == vertex
                        && next != heavy_child_[vertex]) {
                        pending.push_back({next, next});
                    }
                }
            }
        }

        return linear;
    }

    std::vector<int> parent_;
    std::vector<int> depth_;
    std::vector<int> subtree_size_;
    std::vector<int> heavy_child_;
    std::vector<int> head_;
    std::vector<int> position_;
    MaximumTree tree_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertices = 0;
    int operations = 0;
    std::cin >> vertices >> operations;

    std::vector<long long> values(static_cast<std::size_t>(vertices));
    for (long long& value : values) {
        std::cin >> value;
    }

    std::vector<std::vector<int>> graph(
        static_cast<std::size_t>(vertices));
    for (int edge = 1; edge < vertices; ++edge) {
        int first = 0;
        int second = 0;
        std::cin >> first >> second;
        --first;
        --second;
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    HeavyLight roads(graph, values);

    while (operations-- > 0) {
        int type = 0;
        int first = 0;
        long long second = 0;
        std::cin >> type >> first >> second;
        --first;

        if (type == 1) {
            roads.assign(first, second);
        } else {
            std::cout << roads.path_maximum(
                             first, static_cast<int>(second) - 1)
                      << '\n';
        }
    }
}
