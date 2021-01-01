#include <stdlib.h>

template <size_t R, size_t C, typename T>
class Matrix;

template <size_t L, size_t M, size_t N, typename T>
Matrix<L,N,T> mul_winograd(Matrix<L,M,T> &m1, Matrix<M,N,T> &m2)
{
    Matrix<L,N,T> r;
    T sum, mulH;
0   T mulV[N] = {};
0   T* m1_i = m1[0];
0   size_t M_ = M >> 1 << 1;


1   for (int j = 0; j < N; j++) {
2       sum = 0;
3       for (int k = 0;  k < M_; k += 2)
4           sum += m2[k][j] * m2[k+1][j];
5       mulV[j] = sum;
    }


6   for (int i = 0; i < L; i++, m1_i += M) {
7       mulH = 0;
8       for (int k = 0;  k < M_; k += 2)
9           mulH += m1_i[k] * m1_i[k+1];

10      for (int j = 0; j < N; j++) {
11          sum = -mulH - mulV[j];
12          for (int k = 0; k < M_; k += 2)
13              sum += (m1_i[k] + m2[k+1][j])
                    * (m1_i[k+1] + m2[k][j]);
14          r[i][j] = sum;
        }
    }

15      if (M % 2)
16          for (int i = 0; i < L; i++)
17              for (int j = 0; j < N; j++)
18                  r[i][j] += m1[i][M_] * m2[M_][j]

    return r;
}