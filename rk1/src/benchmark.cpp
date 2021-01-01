#include "sparse_matrix.h"
#include <benchmark/benchmark.h>



template <size_t N>
class MaxRow : public benchmark::Fixture
{
public:
    SymSparseMatrix<N> *M;
    void SetUp(const ::benchmark::State &state)
    {
        char path[32];
        sprintf(path, "../data/%ld.txt", N);
        M = new SymSparseMatrix<N>(path);
    }
};

# define BENCH_TEM_F(N) \
BENCHMARK_TEMPLATE_F(MaxRow, Basic, N) (benchmark::State &st)\
{   for (auto _ : st) M->max_row(); }


# define BENCH_TEM_F_MUL(N,n_thread) \
BENCHMARK_TEMPLATE_F(MaxRow, Multithread, N) (benchmark::State &st)\
{   for (auto _ : st) M->max_row(n_thread); }


#define BENCHMARK_MATRIX_SIZE(N)             \
    BENCH_TEM_F(N);      \
    BENCH_TEM_F_MUL(N, 4);
    // BENCH_TEM_F_MUL(N, 1); \
    // BENCH_TEM_F_MUL(N, 2); \
    // BENCH_TEM_F_MUL(N, 4); \

// BENCHMARK_REGISTER_F(MyFixture, DoubleTest)->Threads(2);

BENCHMARK_MATRIX_SIZE(100)
// BENCHMARK_MATRIX_SIZE(200)
// BENCHMARK_MATRIX_SIZE(300)
// BENCHMARK_MATRIX_SIZE(400)
// BENCHMARK_MATRIX_SIZE(500)
// BENCHMARK_MATRIX_SIZE(600)
// BENCHMARK_MATRIX_SIZE(700)
// BENCHMARK_MATRIX_SIZE(800)
// BENCHMARK_MATRIX_SIZE(900)
// BENCHMARK_MATRIX_SIZE(1000)

BENCHMARK_MAIN();
