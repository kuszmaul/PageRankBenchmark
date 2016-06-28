#include "cache-friendly.hh"
#include <cassert>
#include <cstdint>

void test_cache_aware_matrix (void) {
    typedef RCV<uint32_t> RCVu;
    typedef std::tuple<uint32_t, uint32_t> CS;
    {
        std::vector<RCVu> nz = {RCVu(0,0,1),  RCVu(0,1,2),  RCVu(0,2,3),  RCVu(0,3,4),
                                RCVu(1,0,5),  RCVu(1,1,6),  RCVu(1,2,7),  RCVu(1,3,8),
                                RCVu(2,0,9),  RCVu(2,1,10), RCVu(2,2,11), RCVu(2,3,12),
                                RCVu(3,0,13), RCVu(3,1,14), RCVu(3,2,15), RCVu(3,3,16)};
        cache_aware_sparse_matrix<uint32_t> m(4, nz, 1);
        // The resulting matrix layout should look like this
        //   {{0,0,1}, {1,0,5},  {0,1,2},  {1,1,6},  {0,2,3}, {1,2,7},   {0,3,4}, {1,3,8},
        //    {2,0,9}, {3,0,13}, {2,1,10}, {3,1,14}, {2,2,11}, {3,2,15}, {2,3,12}, {3,3,16}
        assert (m.N == 4);
        assert (m.n_batches == 2);
        assert(m.log_rows_per_batch == 1);
        std::vector<uint32_t> batch_starts = {0, 5, 10};
        assert(m.batch_starts == batch_starts);
        std::vector<CS> col_starts = {CS(0,0), CS(1, 2), CS(2, 4), CS(3, 6), CS(4, 8),
                                      CS(0,8), CS(1,10), CS(2,12), CS(3,14), CS(4,16)};
        assert(m.col_starts == col_starts);
        std::vector<uint32_t> expected_rows = {0,1,0,1,0,1,0,1,2, 3, 2, 3, 2, 3, 2, 3};
        std::vector<double> expected_values = {1,5,2,6,3,7,4,8,9,13,10,14,11,15,12,16};
        assert(m.rows == expected_rows);
        assert(m.vals == expected_values);
    }
    {
        std::vector <RCVu> nz = {};
        cache_aware_sparse_matrix<uint32_t> m(5, nz, 1);
        assert(m.N == 5);
        assert (m.n_batches == 3);
        assert(m.log_rows_per_batch == 1);
        std::vector<uint32_t> batch_starts = {0, 1, 2, 3};
        assert(m.batch_starts == batch_starts);
        std::vector<CS> col_starts = {CS(5,0), CS(5,0), CS(5,0)}; // these are all sentinals
        assert(m.col_starts == col_starts);
        assert(m.rows.size() == 0);
        assert(m.vals.size() == 0);
    }
#if 0
    {
        std::vector<RCVu> nz = {RCVu(4,4,1), RCVu(8,4,2)};
        cache_aware_sparse_matrix<uint32_t> m(13, nz, 2);
        assert(m.N == 13);
        assert(m.n_batches == 4);
        assert(m.log_rows_per_batch == 2);
        std::vector<CS> batch1_starts = {CS(4,0)};
        std::vector<CS> batch2_starts = {CS(4,1)};
        std::vector<CS> empty_batch;
        std::vector<std::vector<CS>> col_starts = {empty_batch, batch1_starts, batch2_starts, empty_batch};
        if (0) {
            for (size_t i = 0; i < m.col_starts.size(); i++) {
                printf(" batch %ld:", i);
                for (const CS &cs : m.col_starts[i]) {
                    printf(" %d,%d", std::get<0>(cs), std::get<1>(cs));
                }
                printf("\n");
            }
        }
        assert(m.col_starts == col_starts);
        std::vector<uint32_t> expected_rows = {4, 8};
        std::vector<double>   expected_vals = {1, 2};
        assert(m.rows == expected_rows);
        assert(m.vals == expected_vals);
    }
#endif
}

int main () {
    test_cache_aware_matrix();
}
