template <size_t N, typename T=int>
class SymSparseMatrix
{
private:
    vector<T> AN;
    T IA[N];
    int _max_sum_row_index; // for testing

public:

    SymSparseMatrix(const char* path)
    {
        ifstream file(path);
        string line;
        int a;
        char b;

        getline(file, line);
        stringstream ss(line);
        while (ss >> a)
        {
            AN.push_back(a);
            ss >> b;
        }

        getline(file, line);
        int i = 0;
        stringstream ss(line);
        while (ss >> a)
        {
            IA[i++] = a;
            ss >> b;
        }

        file >> _max_sum_row_index;
    }

    ~SymSparseMatrix() {}


    ostream& display(ostream& os) const
    {
        os << "\n[AN]\n";
        for (auto i : AN) os << i << ',';
        os << "\n[IA]\n";
        for (int i = 0; i < N; i++) os << IA[i] << ',';
        os << '\n';
        return os;
    }

    bool test(int mr)
    {
        return mr == _max_sum_row_index;
    }

    int sum_row(int n)
    {
        int start = (n == 0) ? 0 : IA[n-1];
        int stop = IA[n];
        int sum = 0;

        for (int i = start; i < stop; i++)
            sum += AN[i];

        for (int i = n+1, diff = 2; i < N; i++, diff++)
            if (IA[i] - IA[i-1] >= diff)
                sum += AN[IA[i]-diff];

        return sum;
    }

    int max_row()
    {
        int mr = 0;
        int max = 0;    // only int
        for (int i = 0; i < N; i++)
        {
            int s = sum_row(i);
            if (s > max)
            {
                mr = i;
                max = s;
            }
        }
        return mr;
    }

    int max_row(size_t n_thread)
    {
        if (n_thread > N) n_thread = N;
        int max_sum[n_thread];
        int max_index[n_thread];

        auto f = [&](size_t begin, int inc) {
            int mr = 0;
            int max = 0;
            for (int i = begin; i < N; i += inc)
            {
                int s = sum_row(i);
                if (s > max)
                {
                    mr = i;
                    max = s;
                }
            }
            max_sum[begin] = max;
            max_index[begin] = mr;
        };

        parallelize(f, N, n_thread);

        int mr = max_index[0];
        int max = max_sum[0];
        for (int i = 1; i < n_thread; i++)
        {
            if (max_sum[i] > max)
            {
                mr = max_index[i];
                max = max_sum[i];
            }
        }
        return mr;
    }
};


template<size_t N, typename T=int>
ostream& operator<<(ostream& os, const SymSparseMatrix<N,T>& m)
{
    m.display(os);
    os << '\n';
    return os;
}


// function can run in parallel
using f_parallel_t = std::function<void(size_t begin, size_t end)>;

void parallelize(f_parallel_t f, size_t loop_size, size_t n_thread)
{
    if (n_thread > loop_size)
        n_thread = loop_size;

    size_t block_size = loop_size / n_thread;
    size_t i = 0;

    // + one main thread
    std::vector<std::thread> threads(n_thread-1);

    for (i = 0; i < n_thread-1; i++)
        threads[i] = std::thread(f, i, n_thread);

    // main thread
    f(i, n_thread);

    for (auto& thread : threads)
        thread.join();
}
