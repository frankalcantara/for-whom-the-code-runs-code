// cap06/01-stack-queue.cpp
// Stack, queue, and monotonic deque -- fundamental contest patterns.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o stack_queue 01-stack-queue.cpp

#include <algorithm>
#include <cassert>
#include <deque>
#include <print>
#include <queue>
#include <stack>
#include <vector>

using Vi = std::vector<int>;
using i64 = long long;

// ============================================================
// Monotonic Stack: Next Greater Element
// For each a[i], finds the next value greater than a[i].
// Invariant: the stack stores indices in decreasing value order.
// Complexity: O(n) -- each element is pushed and popped exactly once.
// ============================================================
std::vector<int> next_greater(const Vi& a) {
    int n = static_cast<int>(a.size());
    std::vector<int> ans(n, -1);  // -1 = does not exist
    std::stack<int> stk;           // indices

    for (int i = 0; i < n; ++i) {
        // a[i] is greater than the top values, so it resolves pending indices.
        while (!stk.empty() && a[stk.top()] < a[i]) {
            ans[stk.top()] = a[i];
            stk.pop();
        }
        stk.push(i);
    }
    return ans;
}

// ============================================================
// Monotonic Stack: Largest Rectangle in Histogram
// For each bar, finds the maximum width with that height.
// O(n) with a stack of increasing heights.
// ============================================================
i64 largest_rectangle(const Vi& heights) {
    int n = static_cast<int>(heights.size());
    std::stack<int> stk;
    i64 best = 0;

    for (int i = 0; i <= n; ++i) {
        int h = (i == n) ? 0 : heights[i];
        while (!stk.empty() && heights[stk.top()] > h) {
            int height = heights[stk.top()]; stk.pop();
            int width  = stk.empty() ? i : i - stk.top() - 1;
            best = std::max(best, (i64)height * width);
        }
        stk.push(i);
    }
    return best;
}

// ============================================================
// Monotonic Queue: Sliding Window Minimum (chapter 05 review)
// Included here to show the pattern with std::deque explicitly.
// ============================================================
Vi sliding_min_queue(const Vi& a, int k) {
    int n = static_cast<int>(a.size());
    Vi res;
    std::deque<int> dq;  // indices; a[dq.front()] = minimum

    for (int i = 0; i < n; ++i) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && a[dq.back()] >= a[i])  dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}

// ============================================================
// Queue with O(1) amortized minimum using two stacks.
// std::queue has no minimum access; this structure adds it.
// ============================================================
struct MinQueue {
    struct Item { int val, mn; };
    std::stack<Item> push_stk, pop_stk;

    void push(int x) {
        int mn = push_stk.empty() ? x : std::min(x, push_stk.top().mn);
        push_stk.push({x, mn});
    }

    void pop() {
        if (pop_stk.empty()) {
            while (!push_stk.empty()) {
                int v = push_stk.top().val; push_stk.pop();
                int mn = pop_stk.empty() ? v : std::min(v, pop_stk.top().mn);
                pop_stk.push({v, mn});
            }
        }
        pop_stk.pop();
    }

    int min() const {
        if (push_stk.empty()) return pop_stk.top().mn;
        if (pop_stk.empty())  return push_stk.top().mn;
        return std::min(push_stk.top().mn, pop_stk.top().mn);
    }

    bool empty() const { return push_stk.empty() && pop_stk.empty(); }
};

int main() {
    // Next Greater Element
    Vi a = {2, 1, 5, 6, 2, 3};
    auto ng = next_greater(a);
    assert((ng == Vi{5, 5, 6, -1, 3, -1}));
    std::print("next greater: ");
    for (int x : ng) std::print("{} ", x);
    std::print("\n");

    // Largest rectangle
    assert(largest_rectangle({2,1,5,6,2,3}) == 10);
    assert(largest_rectangle({2,4})          == 4);
    std::print("largest rectangle: {}\n", largest_rectangle({2,1,5,6,2,3}));

    // Sliding Min Queue
    auto sm = sliding_min_queue({3,1,2,4,1,5}, 3);
    assert((sm == Vi{1, 1, 1, 1}));
    std::print("sliding min k=3: ");
    for (int x : sm) std::print("{} ", x);
    std::print("\n");

    // MinQueue
    MinQueue mq;
    mq.push(3); mq.push(1); mq.push(4);
    assert(mq.min() == 1);
    mq.pop();  // remove 3
    assert(mq.min() == 1);
    mq.pop();  // remove 1
    assert(mq.min() == 4);
    std::print("MinQueue: ok\n");

    std::print("01-stack-queue: all assertions passed\n");
    return 0;
}
