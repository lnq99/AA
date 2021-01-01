#include "sparse_matrix.h"
#include <iostream>

#define MAX_THREAD  8


template <size_t N>
auto measure(SymSparseMatrix<N> M, int n_thread=0)
{
    auto start { std::chrono::steady_clock::now() };
    if (n_thread)
        for (int i = 0; i < 400; i++) M.max_row(n_thread);
    else
        for (int i = 0; i < 400; i++) M.max_row();
    auto stop { std::chrono::steady_clock::now() };
    return (std::chrono::duration_cast<std::chrono::microseconds>(stop - start)).count() / 400;
}


template <size_t N, size_t N_THREAD = MAX_THREAD>
void benchmark(long long *res)
{
    char path[32];
    sprintf(path, "../data/%ld.txt", N);
    SymSparseMatrix<N> M(path);
    cout << N << ",";
    res[0] = measure(M);
    size_t n_thread = 1;
    for (int i = 1; n_thread <= N_THREAD; i++, n_thread <<= 1)
        res[i] = measure(M, n_thread);
}


#define BENCH(N, res)   \
benchmark<N>(res);      \
for (auto i: res)       \
std::cout << i << ",";  \
std::cout << std::endl;


int main()
{
    {
        cout << "\n=== Benchmark ===\n";
        long long res[5];
        BENCH(100, res);
        BENCH(200, res);
        BENCH(300, res);
        BENCH(400, res);
        BENCH(500, res);
        BENCH(600, res);
        BENCH(700, res);
        BENCH(800, res);
        BENCH(900, res);
        BENCH(1000, res);
    }

    {
        cout << "\n=== Program ===\n";
        SymSparseMatrix<10> M("../data/10.txt");

        cout << M;
        cout << M.max_row() << endl;
    }

    {
        cout << "\n=== Testing ===\n";
        cout << boolalpha;
        SymSparseMatrix<100> M("../data/100.txt");

        cout << "Not parallelize ";
        cout << M.test(M.max_row()) << endl;
        for (int i = 1; i < 10; i ++)
        {
            cout << i << " threads ";
            cout << M.test(M.max_row(i)) << endl;
        }
    }

    return 0;
}