/* Pagerank Pipeline Benchmark in C++                          */
/* Copyright 2015 Bradley C. Kuszmaul, bradley@mit.edu         */

#ifndef PAGERANKPIPELINE_HH
#define PAGERANKPIPELINE_HH

#include "csc.hh"
#include <cstdio>
#include <vector>

template <class T>
void pagerankpipeline(int SCALE, int edges_per_vertex, int n_files, int n_iterations);

extern FILE *data_file; // set to non-NULL if you want a data file suitable for gnuplot.

template <class T, class Matrix>
using k3_once_t = void (*)(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a,
                           const Matrix &M, std::vector<double> &r, std::vector<double> &r2);

template <class T>
void k3_once_base(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a, const csc_matrix<T> &M,
                  std::vector<double> &r, std::vector<double> &r2);

template <class T, int basesize>
void k3_once_cilk(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a, const csc_matrix<T> &M,
                  std::vector<double> &r, std::vector<double> &r2);

template <class T>
void k3_once_cilkfor(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a, const csc_matrix<T> &M,
                     std::vector<double> &r, std::vector<double> &r2);

template <class T>
void k3_once_omp(const size_t Nbegin, const size_t Nend, const double c, const double fsum, const double a, const csc_matrix<T> &M,
                 std::vector<double> &r, std::vector<double> &r2);



// Internal functions exported for testing:
template <class T, class Matrix, k3_once_t<T, Matrix> k3_once>
std::vector<double> kernel3_compute(const int SCALE, 
                                    const Matrix &M,
                                    const int n_iterations,
                                    // for testing we use a known r.  In production pass nullptr.
                                    std::vector<double> *initial_r);

// Local Variables:
// mode: C++
// c-basic-offset: 2
// indent-tabs-mode: nil
// End:

#endif
