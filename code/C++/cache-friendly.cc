// A divide-and-conquer matrix representation can handle the various kinds of cache friendlyness.

#include "cache-friendly.hh"
#include <algorithm>
#include <cassert>

template <class T>
cache_aware_sparse_matrix<T>::cache_aware_sparse_matrix(
    size_t N, std::vector<RCV<T>> nonzeros, size_t log_rows_per_batch)
        : N(N)
        , log_rows_per_batch(log_rows_per_batch)
        , col_starts(std::vector<std::vector<std::tuple<T,T>>>((N+log_rows_per_batch-1)>>log_rows_per_batch, std::vector<std::tuple<T,T>>(0)))
        , rows(std::vector<T>(nonzeros.size()))
        , vals(std::vector<double>(nonzeros.size()))
{
    assert(nonzeros.size() <= std::numeric_limits<T>::max());
    for (const RCV<T> &nz : nonzeros) {
        assert(0 <= nz.row && nz.row < N);
        assert(0 <= nz.col && nz.col < N);
    }
    // Nonzeros may be sorted into an arbitrary order. We'll sort
    // them again now.
    std::sort(nonzeros.begin(), nonzeros.end(), RCV_sort_cache_aware_predicate<uint32_t>(log_rows_per_batch));
    ssize_t prev_row = -1, prev_col = -1;
    for (size_t i = 0; i < nonzeros.size(); i++) {
        T row = nonzeros[i].row;
        T col = nonzeros[i].col;
        T batch = batch_of_row(row, log_rows_per_batch);
        rows[i] = row;
        vals[i] = nonzeros[i].val;
        assert(row != prev_row || col != prev_col); // cannot have the same row and column twice
        if (prev_row == -1            // It's the first nonzero.
            || col != prev_col        // Or it's a new column
            || batch_of_row(prev_row, log_rows_per_batch) != batch // Or it's a new batch
            ) {
            // We know i will fit in T because of the assertion above.
            col_starts[batch].push_back(std::tuple<T,T>(col, i));
        }
        prev_row = row;
        prev_col = col;
    }
};

template <class T, class Matrix>
void k3_cache_aware(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a, const Matrix &matrix,
                    std::vector<double> &r, std::vector<double> &r2) {
  for (size_t i = Nbegin; i < Nend; i++) {
    // In matlab, this is    r = ((c .* r) * M) + (a .* sum(r,2))
    double dotsum = 0;
#if 0
    const T start_col = matrix.col_starts[i];
    const T end_col   = M.col_starts[i+1];
    for (T vi = start_col; vi < end_col; vi++) {
      dotsum += r[M.rows[vi]] * M.vals[vi];
    }
#endif
    r2[i] = c * dotsum  + a * fsum;
  }
}

template class cache_aware_sparse_matrix<uint32_t>;
