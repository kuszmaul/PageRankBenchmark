#ifndef CACHE_FRIENDLY_HH
#define CACHE_FRIENDLY_HH

#include <cstddef>
#include <vector>

template <class T>
struct RCV {
    T row, col;
    double val;
    RCV(T row, T col, double val) 
            :row(row)
            ,col(col)
            ,val(val)
    {}
};

template <class T>
class cache_aware_sparse_matrix {
    // Divide things into batches of rows.
    // Each batch is several rows stored in column-major order.
    const size_t N;
    const int    log_rows_per_batch;
    std::vector<std::vector<T>> col_starts; // for each batch of rows, where does each column start (index into rows and vals)
    // The rows and vals are just the batches concatenated.
    std::vector<T> rows;
    std::vector<double> vals;
  public:
    cache_aware_sparse_matrix(size_t N, std::vector<RCV<T>> nonzeros, size_t log_rows_per_batch = 10);
    friend void test_cache_aware_matrix (void);
};

#endif
