#include <algorithm>
#include <iostream>
#include <vector>

struct Orientation {
    long long height;
    long long width;
    long long depth;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int types = 0;
    std::cin >> types;

    std::vector<Orientation> boxes;
    boxes.reserve(static_cast<std::size_t>(3 * types));

    const auto add_orientation = [&](long long height,
                                     long long first,
                                     long long second) {
        if (first > second) {
            std::swap(first, second);
        }
        boxes.push_back({height, first, second});
    };

    for (int i = 0; i < types; ++i) {
        long long height = 0;
        long long width = 0;
        long long depth = 0;
        std::cin >> height >> width >> depth;
        add_orientation(height, width, depth);
        add_orientation(width, height, depth);
        add_orientation(depth, height, width);
    }

    std::ranges::sort(boxes, [](const Orientation& a, const Orientation& b) {
        return a.width * a.depth > b.width * b.depth;
    });

    std::vector<long long> best(boxes.size());
    long long answer = 0;
    for (std::size_t top = 0; top < boxes.size(); ++top) {
        best[top] = boxes[top].height;
        for (std::size_t below = 0; below < top; ++below) {
            if (boxes[below].width > boxes[top].width
                && boxes[below].depth > boxes[top].depth) {
                best[top] =
                    std::max(best[top], best[below] + boxes[top].height);
            }
        }
        answer = std::max(answer, best[top]);
    }

    std::cout << answer << '\n';
}
