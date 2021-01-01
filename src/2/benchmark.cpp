#include "matrix/matrix.h"
#include <benchmark/benchmark.h>

#define N_LOOP  1
#define LOOP(X) for (int _ = 0; _ < X; _++)
#define BENCH   for (auto _ : state) LOOP(N_LOOP)


template <size_t N>
static void BM_MulStandard(benchmark::State &state)
{
    Matrix<N, N> A, B;
    BENCH A * B;
}

template <size_t N>
static void BM_MulWinograd(benchmark::State &state)
{
    Matrix<N, N> A, B;
    BENCH A ^ B;
}

template <size_t N>
static void BM_MulWinogradOpt(benchmark::State &state)
{
    Matrix<N, N> A, B;
    BENCH A & B;
}


#define BENCHMARK_MATRIX_SIZE(N) \
BENCHMARK_TEMPLATE(BM_MulStandard, N);  \
BENCHMARK_TEMPLATE(BM_MulWinograd, N);  \
BENCHMARK_TEMPLATE(BM_MulWinogradOpt, N);


BENCHMARK_MATRIX_SIZE(100)
BENCHMARK_MATRIX_SIZE(200)
BENCHMARK_MATRIX_SIZE(300)
BENCHMARK_MATRIX_SIZE(400)
BENCHMARK_MATRIX_SIZE(500)
BENCHMARK_MATRIX_SIZE(600)
BENCHMARK_MATRIX_SIZE(700)
BENCHMARK_MATRIX_SIZE(800)

BENCHMARK_MATRIX_SIZE(101)
BENCHMARK_MATRIX_SIZE(201)
BENCHMARK_MATRIX_SIZE(301)
BENCHMARK_MATRIX_SIZE(401)
BENCHMARK_MATRIX_SIZE(501)
BENCHMARK_MATRIX_SIZE(601)
BENCHMARK_MATRIX_SIZE(701)
BENCHMARK_MATRIX_SIZE(801)


BENCHMARK_MAIN();
