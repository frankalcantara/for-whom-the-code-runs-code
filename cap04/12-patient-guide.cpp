// Exercise 4.3 - The Patient Guide
// Maximize scenic value earned from the guide's accumulated time advantage.
// Time: O(k). Space: O(k).

#include <algorithm>
#include <iostream>
#include <print>
#include <vector>

struct Viewpoint {
    long long distance;
    long long value;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long trail_length = 0;
    int k = 0;
    long long group_rate = 0;
    long long guide_rate = 0;
    std::cin >> trail_length >> k >> group_rate >> guide_rate;
    static_cast<void>(trail_length);

    std::vector<Viewpoint> points(k);
    for (auto& point : points) std::cin >> point.distance >> point.value;

    std::vector<Viewpoint> chosen;
    long long best_to_right = -1;
    for (int i = k - 1; i >= 0; --i) {
        if (points[i].value > best_to_right) {
            chosen.push_back(points[i]);
            best_to_right = points[i].value;
        }
    }
    std::ranges::reverse(chosen);

    long long answer = 0;
    long long previous_distance = 0;
    const long long advantage_per_meter = group_rate - guide_rate;

    for (const auto& point : chosen) {
        const long long new_advantage =
            (point.distance - previous_distance) * advantage_per_meter;
        answer += new_advantage * point.value;
        previous_distance = point.distance;
    }

    std::print("{}\n", answer);
}
