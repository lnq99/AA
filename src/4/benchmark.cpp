#include "matrix/matrix.h"
#include <benchmark/benchmark.h>

#define BENCH   for (auto _ : state)
#define BENCHMARK_TEMPLATE_Multithreading(N, N_THREAD)    \
BENCHMARK_TEMPLATE(BM_Multithreading, N, N_THREAD)->UseRealTime()


template <size_t N>
static void BM_Winograd(benchmark::State &state)
{
    Matrix<N, N> A, B;
    BENCH mul_winograd(A, B);
}

template <size_t N, size_t N_THREAD = 1>
static void BM_Multithreading(benchmark::State &state)
{
    Matrix<N, N> A, B;
    BENCH mul_winograd_multithread(A, B, N_THREAD);
}


#define BENCHMARK_MATRIX_SIZE(N) \
BENCHMARK_TEMPLATE(BM_Winograd, N); \
BENCHMARK_TEMPLATE_Multithreading(N, 1);\
BENCHMARK_TEMPLATE_Multithreading(N, 2);\
BENCHMARK_TEMPLATE_Multithreading(N, 4);\
BENCHMARK_TEMPLATE_Multithreading(N, 8);


BENCHMARK_MATRIX_SIZE(100)
BENCHMARK_MATRIX_SIZE(200)
BENCHMARK_MATRIX_SIZE(300)
BENCHMARK_MATRIX_SIZE(400)
BENCHMARK_MATRIX_SIZE(500)
BENCHMARK_MATRIX_SIZE(600)
BENCHMARK_MATRIX_SIZE(700)
BENCHMARK_MATRIX_SIZE(800)

BENCHMARK_MAIN();
