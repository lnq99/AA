template <size_t R, size_t C, typename T = int>
class Matrix : public BaseMatrix
{
private:
    T data[R][C];
    // ...
}


template <size_t L, size_t M, size_t N, typename T>
Matrix<L,N,T> mul_winograd(
    Matrix<L,M,T> &m1,
    Matrix<M,N,T> &m2)
{
    Matrix<L,N,T> r;
    T sum, mulH;
    T mulV[N] = {};
    size_t M_ = M >> 1 << 1;


    for (int j = 0; j < N; j++)
    {
        sum = 0;
        for (int k = 0;  k < M_; k += 2)
            sum += m2[k][j] * m2[k+1][j];
        mulV[j] = sum;
    }

    T* m1_i = m1[0];

    for (int i = 0; i < L; i++, m1_i += M)
    {
        mulH = 0;
        for (int k = 0;  k < M_; k += 2)
            mulH += m1_i[k] * m1_i[k+1];

        for (int j = 0; j < N; j++)
        {
            sum = -mulH - mulV[j];
            for (int k = 0; k < M_; k += 2)
                sum += (m1_i[k] + m2[k+1][j])
                    * (m1_i[k+1] + m2[k][j]);

            if (M % 2)
                sum += m1_i[M_] * m2[M_][j];

            r[i][j] = sum;
        }
    }

    return r;
}





using f_parallel_t = std::function<void(size_t begin, size_t end)>;

void parallelize(f_parallel_t f, size_t loop_size, size_t n_thread)
{
    if (n_thread > loop_size)
        n_thread = loop_size;

    size_t block_size = loop_size / n_thread;
    size_t begin = 0;

    // + one main thread
    n_thread--;
    std::vector<std::thread> threads(n_thread);

    for (size_t i = 0; i < n_thread; i++, begin += block_size)
        threads[i] = std::thread(f, begin, begin + block_size);

    // main thread
    f(begin, loop_size);

    for (auto& thread : threads)
        thread.join();
}


template <size_t L, size_t M, size_t N, typename T>
Matrix<L,N,T> mul_winograd_multithread(
    Matrix<L,M,T> &m1,
    Matrix<M,N,T> &m2,
    size_t n_thread = 1)
{
    Matrix<L,N,T> r;
    T mulV[N] = {};
    size_t M_ = M >> 1 << 1;

    auto fMulV = [&](size_t begin, size_t end) {
        for (int j = begin; j < end; j++)
        {
            T sum = 0;
            for (int k = 0;  k < M_; k += 2)
                sum += m2[k][j] * m2[k+1][j];
            mulV[j] = sum;
        }
    };

    auto fMulMat = [&](size_t begin, size_t end) {
        T* m1_i = m1[begin];
        for (int i = begin; i < end; i++, m1_i += M)
        {
            T mulH = 0;
            for (int k = 0;  k < M_; k += 2)
                mulH += m1_i[k] * m1_i[k+1];

            for (int j = 0; j < N; j++)
            {
                T sum = -mulH - mulV[j];
                for (int k = 0; k < M_; k += 2)
                    sum += (m1_i[k] + m2[k+1][j])
                        * (m1_i[k+1] + m2[k][j]);

                if (M % 2)
                    sum += m1_i[M_] * m2[M_][j];

                r[i][j] = sum;
            }
        }
    };

    // fMulV(0, N);
    // fMulMat(0, L);
    parallelize(fMulV, N, n_thread);
    parallelize(fMulMat, L, n_thread);

    return r;
}