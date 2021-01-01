#include "matrix/matrix.h"
#include "utils.h"
#include <iostream>

#define MAX_THREAD  8

template <size_t N, size_t N_THREAD = MAX_THREAD>
void benchmark(long long *res)
{
    Matrix<N,N> A, B;
    res[0] = utils::measure(mul_winograd<N,N,N,int>, A, B);
    size_t n_thread = 1;
    for (int i = 1; n_thread <= N_THREAD; i++, n_thread <<= 1)
        res[i] = utils::measure(mul_winograd_multithread<N,N,N,int>, A, B, n_thread);
}

#define BENCH(N, res)   \
benchmark<N>(res);      \
for (auto i: res)       \
std::cout << i << ",";  \
std::cout << std::endl;


int main(int argc, char** argv)
{
    if (argc == 2 && (!strcmp(argv[1], "-b")))
    {
        long long res[5];
        BENCH(100, res);
        BENCH(200, res);
        BENCH(300, res);
        BENCH(400, res);
        BENCH(500, res);
        BENCH(600, res);
        BENCH(700, res);
        BENCH(800, res);

        return 0;
    }

    Matrix<2,3> A = {
        2,4,3,
        1,-3,2
    };
    Matrix<3,2> B = {
        2,-3,
        4,4,
        2,3
    };

    std::cout << "A " << A << "B " << B;

    std::cout << "Coppersmith–Winograd optimized\n";
    std::cout << "AxB " << (A * B);
    std::cout << "BxA " << (B * A);

    std::cout << "Coppersmith–Winograd multithreading\n";
    std::cout << "AxB " << (mul_winograd_multithread(A, B));
    std::cout << "BxA " << (mul_winograd_multithread(B, A));

    return 0;
}