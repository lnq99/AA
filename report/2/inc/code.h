template <size_t R, size_t C, typename T = int>
class Matrix : public BaseMatrix
{
private:
    T data[R][C];
    // ...
}


    template <size_t R2, size_t C2>
    Matrix<R,C2,T> operator*(Matrix<R2,C2,T> &m2)
    {
        if (C != R2) throw std::exception();

        Matrix<R,C2,T> r;

        for (int i = 0; i < R; i++)
        {
            for (int j = 0; j < C2; j++)
            {
                T sum = 0;
                for (int k = 0; k < C; k++)
                    sum += data[i][k] * m2[k][j];

                r[i][j] = sum;
            }
        }

        return r;
    }

    template <size_t R2, size_t C2>
    Matrix<R,C2,T> operator^(Matrix<R2,C2,T> &m2)
    {
        if (C != R2) throw std::exception();

        Matrix<R,C2,T> r
        T mulH[R] = {}, mulV[C2] = {};
        T sum;

        for (int i = 0; i < R; i++)
        {
            sum = 0;
            for (int k = 0; k < C/2; k++)
                sum += data[i][2*k] * data[i][2*k+1];
            mulH[i] = sum;
        }

        for (int j = 0; j < C2; j++)
        {
            sum = 0;
            for (int k = 0; k < C/2; k++)
                sum += m2[2*k][j] * m2[2*k+1][j];
            mulV[j] = sum;
        }

        for (int i = 0; i < R; i++)
        {
            for (int j = 0; j < C2; j++)
            {
                sum = -mulH[i] - mulV[j];
                for (int k = 0; k < C/2; k++)
                    sum += (data[i][2*k] + m2[2*k+1][j])
                        * (data[i][2*k+1] + m2[2*k][j]);
                r[i][j] = sum;
            }
        }

        if (C % 2)
        {
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C2; j++)
                    r[i][j] += data[i][C-1] * m2[C-1][j];
        }

        return r;
    }


    template <size_t R2, size_t C2>
    Matrix<R,C2,T> operator&(Matrix<R2,C2,T> &m2)
    {
        if (C != R2) throw std::exception();

        Matrix<R,C2,T> r;
        T mulH, mulV[C2] = {};
        T sum;
        size_t C_ = C >> 1 << 1;

        for (int j = 0; j < C2; j++)
        {
            sum = 0;
            for (int k = 0;  k < C_; k += 2)
                sum += m2[k][j] * m2[k+1][j];
            mulV[j] = sum;
        }

        T* m1_i = data[0];

        for (int i = 0; i < R; i++, m1_i += C)
        {
            mulH = 0;
            for (int k = 0;  k < C_; k += 2)
                mulH += m1_i[k] * m1_i[k+1];

            for (int j = 0; j < C2; j++)
            {
                sum = -mulH - mulV[j];
                for (int k = 0; k < C_; k += 2)
                    sum += (m1_i[k] + m2[k+1][j])
                        * (m1_i[k+1] + m2[k][j]);

                if (C % 2)
                    sum += m1_i[C_] * m2[C_][j];
                r[i][j] = sum;
            }
        }

        return r;
    }