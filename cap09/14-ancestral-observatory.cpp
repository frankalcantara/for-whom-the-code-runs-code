#include <bit>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

struct Edge {
    int to;
    std::int64_t weight;
};

class Ancestry {
public:
    Ancestry(const std::vector<std::vector<Edge>>& graph, int root)
        : levels_(std::bit_width(
              static_cast<unsigned int>(graph.size()))),
          depth_(graph.size(), 0),
          root_distance_(graph.size(), 0),
          up_(static_cast<std::size_t>(levels_),
              std::vector<int>(graph.size(), -1)) {
        std::vector<int> stack{root};

        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();

            for (const Edge edge : graph[vertex]) {
                if (edge.to == up_[0][vertex]) {
                    continue;
                }

                up_[0][edge.to] = vertex;
                depth_[edge.to] = depth_[vertex] + 1;
                root_distance_[edge.to] =
                    root_distance_[vertex] + edge.weight;
                stack.push_back(edge.to);
            }
        }

        for (int level = 1; level < levels_; ++level) {
            for (std::size_t vertex = 0; vertex < graph.size(); ++vertex) {
                const int halfway = up_[level - 1][vertex];
                if (halfway != -1) {
                    up_[level][vertex] = up_[level - 1][halfway];
                }
            }
        }
    }

    [[nodiscard]] int kth_ancestor(int vertex, int steps) const {
        if (steps > depth_[vertex]) {
            return -1;
        }

        for (int level = 0; level < levels_; ++level) {
            if (((steps >> level) & 1) != 0) {
                vertex = up_[level][vertex];
            }
        }
        return vertex;
    }

    [[nodiscard]] int lca(int first, int second) const {
        if (depth_[first] < depth_[second]) {
            std::swap(first, second);
        }

        first = kth_ancestor(first, depth_[first] - depth_[second]);
        if (first == second) {
            return first;
        }

        for (int level = levels_; level-- > 0;) {
            if (up_[level][first] != up_[level][second]) {
                first = up_[level][first];
                second = up_[level][second];
            }
        }
        return up_[0][first];
    }

    [[nodiscard]] std::int64_t distance(int first, int second) const {
        const int ancestor = lca(first, second);
        return root_distance_[first] + root_distance_[second]
               - 2 * root_distance_[ancestor];
    }

private:
    int levels_;
    std::vector<int> depth_;
    std::vector<std::int64_t> root_distance_;
    std::vector<std::vector<int>> up_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertices = 0;
    int queries = 0;
    int root = 0;
    std::cin >> vertices >> queries >> root;
    --root;

    std::vector<std::vector<Edge>> graph(
        static_cast<std::size_t>(vertices));
    for (int i = 1; i < vertices; ++i) {
        int first = 0;
        int second = 0;
        std::int64_t weight = 0;
        std::cin >> first >> second >> weight;
        --first;
        --second;
        graph[first].push_back({second, weight});
        graph[second].push_back({first, weight});
    }

    const Ancestry ancestry(graph, root);

    for (int query = 0; query < queries; ++query) {
        char type = '\0';
        int first = 0;
        int value = 0;
        std::cin >> type >> first >> value;
        --first;

        if (type == 'L') {
            --value;
            std::cout << ancestry.lca(first, value) + 1 << '\n';
        } else if (type == 'D') {
            --value;
            std::cout << ancestry.distance(first, value) << '\n';
        } else {
            const int answer = ancestry.kth_ancestor(first, value);
            std::cout << (answer == -1 ? -1 : answer + 1) << '\n';
        }
    }
}
