// A divide-and-conquer matrix representation can handle the various kinds of cache friendlyness.

#include "cache-friendly.hh"
#include <algorithm>
#include <cassert>

template <class T>
cache_aware_sparse_matrix<T>::cache_aware_sparse_matrix(
    T N, std::vector<RCV<T>> nonzeros, int log_rows_per_batch)
    : N(N)
    , log_rows_per_batch(log_rows_per_batch)
    , n_batches((N+(1ul<<log_rows_per_batch)-1) >> log_rows_per_batch)
    , batch_starts(std::vector<T>(n_batches+1))
    , col_starts(std::vector<std::tuple<T,T>>(0))
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
  ssize_t prev_row = -1, prev_col = -1, prev_batch = -1;
  for (size_t i = 0; i < nonzeros.size(); i++) {
    T row = nonzeros[i].row;
    T col = nonzeros[i].col;
    T batch = batch_of_row(row, log_rows_per_batch);

    if (prev_batch >= 0 && prev_batch != batch) {
      col_starts.push_back(std::tuple<T,T>(N, i)); // sentinal at the end
      prev_col = N;
    }
    while (prev_batch < batch) {
      prev_batch++;
      batch_starts[prev_batch] = col_starts.size();
      printf("Batch %ld starts at %d\n", prev_batch, batch_starts[prev_batch]);
      assert (prev_col != col); // we are about to push a col
    }
    if (prev_col != col) {
      col_starts.push_back(std::tuple<T,T>(col, i));
    }
    rows[i] = row;
    vals[i] = nonzeros[i].val;
    assert(row != prev_row || col != prev_col); // cannot have the same row and column twice
    prev_row = row;
    prev_col = col;
    prev_batch = batch;
  }
  while (prev_batch < n_batches) {
    prev_batch++;
    batch_starts[prev_batch] = col_starts.size() + 1;
    col_starts.push_back(std::tuple<T,T>(N, nonzeros.size()));
    printf("finally Batch %ld starts at %d\n", prev_batch, batch_starts[prev_batch]);
  }
};

#if 0
template <class T>
void k3_cache_aware(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a, 
                    const cache_aware_sparse_matrix<T> &matrix,
                    std::vector<double> &r, std::vector<double> &r2) {
  for (const std::vector<std::tuple<T, T>> & batch : matrix.col_starts) {
    for (size_t i = 0; i+1 < batch.size(); i++) {
      const std::tuple<T, T> &cs = batch[i];
      const std::tuple<T, T> &nextcs = batch[i+1];
      const T row = std::get<0>(cs);
      const T thiscol = std::get<1>(cs);
      const T nextcol = std::get<1>(nextcs);
      double dotsum = 0;
      for (size_t vi = this_col;
    }
  }

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
#endif

template class cache_aware_sparse_matrix<uint32_t>;


// Local Variables:
// mode: C++
// c-basic-offset: 2
// indent-tabs-mode: nil
// End:
