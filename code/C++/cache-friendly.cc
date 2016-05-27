// A divide-and-conquer matrix representation can handle the various kinds of cache friendlyness.

#include "cache-friendly.hh"
#include <algorithm>
#include <cassert>

template <class T>
cache_aware_sparse_matrix<T>::cache_aware_sparse_matrix(
    size_t N, std::vector<RCV<T>> nonzeros, size_t log_rows_per_batch)
        : N(N)
        , log_rows_per_batch(log_rows_per_batch)
        , rows(std::vector<T>(nonzeros.size()))
        , vals(std::vector<double>(nonzeros.size()))
{
    // Nonzeros may be sorted into an arbitrary order. We'll sort
    // them again now.
    std::sort(nonzeros.begin(), nonzeros.end(), 
              [log_rows_per_batch] (const RCV<T> &a, const RCV<T> &b) {
                  T arow = a.row;
                  T brow = b.row;
                  T abatch = arow >> log_rows_per_batch;
                  T bbatch = brow >> log_rows_per_batch;
                  if (abatch < bbatch) return true;
                  if (bbatch < abatch) return false;
                  // They are in the same batch, so sort by column major
                  T acol = a.col;
                  T bcol = b.col;
                  if (acol < bcol) return true;
                  if (bcol > acol) return false;
                  // They are in the batch and the same column, so the row determins
                  return arow < brow;
              });
    for (size_t i = 0; i < nonzeros.size(); i++) {
        rows[i] = nonzeros[i].row;
        vals[i] = nonzeros[i].val;
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
