// Exercise A5.7 - The Alchemist's Shelf
// Session: A5, exercise 7 of 7 (6 hours for the evaluation)
// Theme: chapters 10 + 06, integration (linear sieve with smallest
//   prime factor for factorization; DSU for the transitive closure)
// Tags: chapter-10, chapter-06, dsu, number-theory, sieve, gcd
//
// MODEL: connected components in an implicit graph: vertices are the
//   reagents, and two reagents are adjacent iff gcd(a_i, a_j) > 1.
//   Materializing the edges is the trap - n^2 gcd tests is 4e10 at
//   the limits. The factorization view dissolves it: two numbers
//   share a divisor > 1 iff they share a PRIME, so it is enough to
//   connect each reagent to its primes, and primes need no vertices
//   of their own: a map from prime -> first reagent seen carrying it,
//   plus one union per later carrier, yields the same components.
// MATH: factoring each a_i by trial division costs O(sqrt(a)) per
//   number; with a smallest-prime-factor table (linear sieve, chapter
//   10) it costs O(log a) - a number <= 1e6 has at most 7 distinct
//   primes (2*3*5*7*11*13*17 = 510510 is the last product that fits).
//   So the union stream has at most 7n = 1.4e6 operations on a DSU of
//   n reagents.
// PROOF: (shared prime <=> shared divisor > 1) is immediate from
//   unique factorization: a common divisor d > 1 has a prime factor,
//   and that prime divides both numbers. For the components: union by
//   shared prime generates exactly the relation "linked by a chain of
//   direct fusions" because DSU computes the transitive-reflexive
//   closure of the union stream (chapter 6 invariant), and the union
//   stream contains an edge for every shared-prime incidence through
//   the prime's first carrier - which is in the same component as
//   every other carrier, so connectivity through "first carrier"
//   equals connectivity through "any carrier".
// COMPLEXITY: sieve O(A) for A = 1e6; factorizations O(n log A);
//   unions O(n alpha(n) log A) effectively linear. Memory: the SPF
//   table is 1e6 ints = 4 MB, the DSU two arrays of n.
// TYPES: essences fit int (<= 1e6); SPF table entries are int;
//   component sizes <= n = 2e5 fit int. Nothing here strains 32 bits
//   - the width lesson this time is knowing when NOT to reach for
//   long long: 4 MB of int32 SPF is cache-friendlier than 8 MB of
//   wider habit.
// ALTERNATIVES: BFS over an adjacency structure keyed by prime
//   (chapter 09) - equivalent components, more bookkeeping; pairwise
//   gcd with early grouping still quadratic; Pollard rho is for
//   64-bit essences and a different book chapter; merging via
//   "DSU over primes + reagents" as 1e6 + n vertices also works and
//   wastes 1e6 find structures on primes that never appear.
// EDGE CASES: essence 1 (no primes: a family of one, never fused -
//   including with another 1); all essences equal (one family of n);
//   n = 1; prime essences appearing once (families of one); duplicate
//   composite essences; the largest family being a singleton.
//
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o shelf a5-07-alchemists-shelf.cpp
// Compile (MSVC): cl /std:c++latest /O2 /EHsc /W4 /permissive- /Zc:__cplusplus /Feshelf.exe a5-07-alchemists-shelf.cpp

#include <algorithm>
#include <iostream>
#include <print>
#include <unordered_map>
#include <vector>

namespace {

// Disjoint set union with union by size and path halving (chapter 6).
class DisjointSets {
public:
    explicit DisjointSets(int n) : parent_(static_cast<std::size_t>(n)), size_(static_cast<std::size_t>(n), 1) {
        for (int i = 0; i < n; ++i) {
            parent_[static_cast<std::size_t>(i)] = i;
        }
    }

    int find(int x) {
        while (parent_[static_cast<std::size_t>(x)] != x) {
            parent_[static_cast<std::size_t>(x)] =
                parent_[static_cast<std::size_t>(parent_[static_cast<std::size_t>(x)])];
            x = parent_[static_cast<std::size_t>(x)];
        }
        return x;
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size_[static_cast<std::size_t>(a)] < size_[static_cast<std::size_t>(b)]) {
            std::swap(a, b);
        }
        parent_[static_cast<std::size_t>(b)] = a;
        size_[static_cast<std::size_t>(a)] += size_[static_cast<std::size_t>(b)];
    }

    int component_size(int x) { return size_[static_cast<std::size_t>(find(x))]; }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
};

// Linear sieve: spf[v] = smallest prime factor of v (chapter 10).
std::vector<int> smallest_prime_factors(int limit) {
    std::vector<int> spf(static_cast<std::size_t>(limit) + 1, 0);
    std::vector<int> primes;
    for (int v = 2; v <= limit; ++v) {
        if (spf[static_cast<std::size_t>(v)] == 0) {
            spf[static_cast<std::size_t>(v)] = v;
            primes.push_back(v);
        }
        for (const int p : primes) {
            if (p > spf[static_cast<std::size_t>(v)] || static_cast<long long>(p) * v > limit) {
                break;
            }
            spf[static_cast<std::size_t>(p * v)] = p;
        }
    }
    return spf;
}

}  // namespace

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<int> essence(static_cast<std::size_t>(n));
    int max_essence = 1;
    for (auto& a : essence) {
        std::cin >> a;
        max_essence = std::max(max_essence, a);
    }

    const std::vector<int> spf = smallest_prime_factors(max_essence);

    DisjointSets families(n);
    std::unordered_map<int, int> first_carrier;  // prime -> reagent index
    first_carrier.reserve(static_cast<std::size_t>(n) * 2);

    for (int i = 0; i < n; ++i) {
        int v = essence[static_cast<std::size_t>(i)];
        while (v > 1) {
            const int p = spf[static_cast<std::size_t>(v)];
            const auto [it, inserted] = first_carrier.try_emplace(p, i);
            if (!inserted) {
                families.unite(i, it->second);
            }
            while (v % p == 0) {
                v /= p;
            }
        }
    }

    int family_count = 0;
    int largest = 0;
    for (int i = 0; i < n; ++i) {
        if (families.find(i) == i) {
            ++family_count;
            largest = std::max(largest, families.component_size(i));
        }
    }

    std::println("{} {}", family_count, largest);
    return 0;
}
