// cap01/04-cache-locality.cpp
// Matrix multiplication: the performance impact of memory access order.
// Three versions: i-j-k (cache-hostile), i-k-j (cache-friendly), block tiling.
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o cache_locality 04-cache-locality.cpp

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <print>
#include <vector>

using i64  = long long;
using Mat  = std::vector<std::vector<int>>;
using nano = std::chrono::nanoseconds;

// ============================================================
// Version 1: i-j-k order (not optimized).
// The inner loop advances k, so B[k][j] walks down a column.
// In row-major C++ storage, this is strided access: each step jumps n integers.
// For n=512, the stride is 2 KB. Most accesses miss the cache.
// ============================================================
Mat matmul_ijk(const Mat& A, const Mat& B) {
    int n = static_cast<int>(A.size());
    Mat C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];  // B[k][j]: stride n
    return C;
}

// ============================================================
// Version 2: i-k-j order (cache-friendly).
// The inner loop advances j, so C[i][j] and B[k][j] are sequential.
// A[i][k] is loaded once and reused as a scalar.
// ============================================================
Mat matmul_ikj(const Mat& A, const Mat& B) {
    int n = static_cast<int>(A.size());
    Mat C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k) {
            int a = A[i][k];
            for (int j = 0; j < n; ++j)
                C[i][j] += a * B[k][j]; // C[i][j] e B[k][j]: ambos sequenciais
        }
    return C;
}

// ============================================================
// Version 3: block tiling.
// Split A, B, and C into TILE by TILE blocks.
// The goal is to keep the active blocks in cache long enough to reuse them.
// ============================================================
static constexpr int TILE = 64;

Mat matmul_tiling(const Mat& A, const Mat& B) {
    int n = static_cast<int>(A.size());
    Mat C(n, std::vector<int>(n, 0));
    for (int ii = 0; ii < n; ii += TILE)
        for (int kk = 0; kk < n; kk += TILE)
            for (int jj = 0; jj < n; jj += TILE)
                for (int i = ii; i < std::min(ii + TILE, n); ++i)
                    for (int k = kk; k < std::min(kk + TILE, n); ++k) {
                        int a = A[i][k];
                        for (int j = jj; j < std::min(jj + TILE, n); ++j)
                            C[i][j] += a * B[k][j];
                    }
    return C;
}

// Benchmark: return the median time of three calls, in milliseconds.
template<typename F>
double bench_ms(F&& fn, int n) {
    std::array<double, 3> samples{};
    for (auto& s : samples) {
        // Reinitialize matrices for each measurement.
        Mat A(n, std::vector<int>(n, 1));
        Mat B(n, std::vector<int>(n, 1));
        auto t0 = std::chrono::steady_clock::now();
        auto C  = fn(A, B);
        auto t1 = std::chrono::steady_clock::now();
        (void)C;
        s = std::chrono::duration<double, std::milli>(t1 - t0).count();
    }
    std::sort(samples.begin(), samples.end());
    return samples[1]; // mediana
}

int main() {
    // Correctness check for n=4.
    {
        int n = 4;
        Mat A(n, std::vector<int>(n, 1));
        Mat B(n, std::vector<int>(n, 1));
        auto C1 = matmul_ijk(A, B);
        auto C2 = matmul_ikj(A, B);
        auto C3 = matmul_tiling(A, B);
        // A*B with all ones: every cell is n.
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                assert(C1[i][j] == n);
                assert(C2[i][j] == n);
                assert(C3[i][j] == n);
            }
    }

    // Benchmark for n=512.
    constexpr int N = 512;
    std::print("Matrix multiplication {}x{} (3 measurements, median):\n", N, N);

    double t_ijk    = bench_ms(matmul_ijk,    N);
    double t_ikj    = bench_ms(matmul_ikj,    N);
    double t_tiling = bench_ms(matmul_tiling, N);

    std::print("  i-j-k  (cache-hostile):   {:7.1f} ms\n", t_ijk);
    std::print("  i-k-j  (cache-friendly):  {:7.1f} ms  ({:.1f}x faster)\n",
               t_ikj, t_ijk / t_ikj);
    std::print("  tiling (TILE={}):         {:7.1f} ms  ({:.1f}x faster)\n",
               TILE, t_tiling, t_ijk / t_tiling);

    std::print("04-cache-locality: all assertions passed.\n");
    return 0;
}
