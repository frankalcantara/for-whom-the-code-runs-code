// cap02/07-namespaces.cpp
// Demonstration of namespaces in C++23:
//   - Namespace aliases (chr, rng, vw)
//   - Why using namespace std is dangerous (ADL, shadowing)
//   - Scoped using declarations
//   - A project namespace for a contest library (dsu)
//
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o namespaces 07-namespaces.cpp

#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <vector>

// ============================================================
// Namespace aliases shorten paths without polluting the global scope.
// ============================================================
namespace rng = std::ranges;   // rng::sort, rng::find, rng::count ...
namespace vw  = std::views;    // vw::filter, vw::transform, vw::enumerate ...

// ============================================================
// Project namespace: DSU (Disjoint Set Union / Union-Find).
// Avoids collisions with std::find, POSIX find, and other components.
// ============================================================
namespace dsu {

std::vector<int> parent, rnk;  // rnk instead of rank; rank can collide in some contexts

// Initializes n disjoint sets {0}, {1}, ..., {n-1}. O(n).
void init(int n) {
    parent.resize(n);
    rnk.assign(n, 0);
    std::iota(parent.begin(), parent.end(), 0);  // parent[i] = i
}

// Finds the representative of x with path compression. Amortized O(alpha(n)).
int find(int x) {
    while (parent[x] != x)
        x = parent[x] = parent[parent[x]];  // path halving
    return x;
}

// Unites the sets of a and b. Returns false if they were already equal. Amortized O(alpha(n)).
bool unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return false;
    if (rnk[a] < rnk[b]) std::swap(a, b);
    parent[b] = a;
    if (rnk[a] == rnk[b]) ++rnk[a];
    return true;
}

// Checks whether a and b belong to the same set. Amortized O(alpha(n)).
bool same(int a, int b) { return find(a) == find(b); }

}  // namespace dsu

// ============================================================
// Demonstration of a scoped using declaration.
// ============================================================
void demo_using_declaration() {
    using std::sort;      // imports only std::sort in this function
    using std::vector;    // imports only std::vector in this function

    vector<int> v = {5, 3, 1, 4, 2};
    sort(v.begin(), v.end());  // unambiguous: std::sort
    std::print("sorted: ");
    for (int x : v) std::print("{} ", x);
    std::print("\n");
}   // outside this function, sort and vector still need the std:: prefix

// ============================================================
// Demonstration of namespace aliases and a views pipeline.
// ============================================================
void demo_aliases() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // Using aliases: vw:: instead of std::views::, rng:: instead of std::ranges::.
    // Same behavior, shorter path, scope unaffected.
    auto evens_sq = v
        | vw::filter([](int x) { return x % 2 == 0; })   // {2,4,6,8}
        | vw::transform([](int x) { return x * x; });     // {4,16,36,64}

    std::print("even squares: ");
    rng::for_each(evens_sq, [](int x) { std::print("{} ", x); });
    std::print("\n");
}

// ============================================================
// Demonstration of the dsu namespace.
// ============================================================
void demo_dsu() {
    // 6 nodes: 0..5; unite pairs, check connectivity.
    dsu::init(6);

    dsu::unite(0, 1);
    dsu::unite(1, 2);
    dsu::unite(3, 4);

    // 0,1,2 in one set; 3,4 in another; 5 isolated.
    std::print("same(0,2)={} same(0,3)={} same(3,4)={} same(4,5)={}\n",
               dsu::same(0, 2),  // true
               dsu::same(0, 3),  // false
               dsu::same(3, 4),  // true
               dsu::same(4, 5)); // false

    dsu::unite(2, 3);  // unite the two larger groups
    std::print("after unite(2,3): same(0,4)={}\n", dsu::same(0, 4));  // true
}

int main() {
    std::print("=== scoped using declaration ===\n");
    demo_using_declaration();

    std::print("\n=== namespace aliases ===\n");
    demo_aliases();

    std::print("\n=== dsu::namespace ===\n");
    demo_dsu();

    return 0;
}
