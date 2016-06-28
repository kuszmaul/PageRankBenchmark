#ifndef CACHE_FRIENDLY_HH
#define CACHE_FRIENDLY_HH

#include <cstddef>
#include <vector>
#include <tuple>

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
    const T   N;
    const int log_rows_per_batch;
    const T   n_batches;
    std::vector<T> batch_starts;
    std::vector<std::tuple<T, T>> col_starts; // (Since batch-ends tells us where they end we don't need any extras or sentinals here)
    // for each batch of rows, where does each column start (index into rows and vals)  

    // The rows and vals are just the batches concatenated.
    std::vector<T> rows;
    std::vector<double> vals;

  public:
    cache_aware_sparse_matrix(T N, std::vector<RCV<T>> nonzeros, int log_rows_per_batch = 10);
    friend void test_cache_aware_matrix (void);
    static T batch_of_row(T row, int log_rows_per_batch) {
        return row >> log_rows_per_batch;
    }
};

template <class T>
struct RCV_sort_cache_aware_predicate {
    const int log_rows_per_batch;
    RCV_sort_cache_aware_predicate(int log_rows_per_batch) : log_rows_per_batch(log_rows_per_batch) {}
    bool operator() (const RCV<T> &a, const RCV<T> &b) const {
        T arow = a.row;
        T brow = b.row;
        T abatch = cache_aware_sparse_matrix<T>::batch_of_row(arow, log_rows_per_batch);
        T bbatch = cache_aware_sparse_matrix<T>::batch_of_row(brow, log_rows_per_batch);
        if (abatch < bbatch) return true;
        if (bbatch < abatch) return false;
        // They are in the same batch, so sort by column major
        T acol = a.col;
        T bcol = b.col;
        if (acol < bcol) return true;
        if (bcol > acol) return false;
        // They are in the batch and the same column, so the row determins
        return arow < brow;
    }
};

#endif
