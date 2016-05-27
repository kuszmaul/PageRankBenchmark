#include "cache-friendly.hh"
#include <cassert>
#include <cstdint>

void test_cache_aware_matrix (void) {
    typedef RCV<uint32_t> RCVu;
    std::vector<RCVu> nz = {RCVu(0,0,1),  RCVu(0,1,2),  RCVu(0,2,3),  RCVu(0,3,4),
                            RCVu(1,0,5),  RCVu(1,1,6),  RCVu(1,2,7),  RCVu(1,3,8),
                            RCVu(2,0,9),  RCVu(2,1,10), RCVu(2,2,11), RCVu(2,3,12),
                            RCVu(3,0,13), RCVu(3,1,14), RCVu(3,2,15), RCVu(3,3,16)};
    cache_aware_sparse_matrix<uint32_t> m(4, nz, 1);
    // The resulting matrix layout should look like this
    //   {{0,0,1}, {1,0,5},  {0,1,2},  {1,1,6},  {0,2,3}, {1,2,7},   {0,3,4}, {1,3,8},
    //    {2,0,9}, {3,0,13}, {2,1,10}, {3,1,14}, {2,2,11}, {3,2,15}, {2,3,12}, {3,3,16}
    assert (m.N == 4);
    assert(m.log_rows_per_batch == 1);
    std::vector<uint32_t> col0_starts = {0, 2,  4,   6};
    std::vector<uint32_t> col1_starts = {8, 10, 12, 14};
    std::vector<std::vector<uint32_t>> col_starts = {col0_starts, col1_starts};
    assert(m.col_starts == col_starts);
    std::vector<uint32_t> expected_rows = {0,1,0,1,0,1,0,1,2, 3, 2, 3, 2, 3, 2, 3};
    std::vector<double> expected_values = {1,5,2,6,3,7,4,8,9,13,10,14,11,15,12,16};
    assert(m.rows == expected_rows);
    assert(m.vals == expected_values);
}

int main () {
    test_cache_aware_matrix();
}
