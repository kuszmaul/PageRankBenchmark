/* Pagerank Pipeline Benchmark in C++                          */
/* Copyright 2015 Bradley C. Kuszmaul, bradley@mit.edu         */

#include "pagerankpipeline.hh"

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <limits>


static const int min_scale_default = 10;
static const int max_scale_default = 22;
static const int n_iterations_default = 20;

std::string progname;
int min_scale = min_scale_default;
int max_scale = max_scale_default;
int n_iterations  = n_iterations_default;

static void usage() {
  fprintf(stderr, "Usage: %s [--minscale min_scale] [--maxscale max_scale] [--iter n_iterations]\n", progname.c_str());
  fprintf(stderr, "  Runs the page rank pipeline on a range of scales\n");
  fprintf(stderr, "  min_scale defaults to %d\n", min_scale_default);
  fprintf(stderr, "  max_scale defaults to %d\n", max_scale_default);
  fprintf(stderr, "  --iter specifies how many many iterations (default %d)\n", n_iterations_default);
  exit(1);
}

static unsigned int parse_uint(const char *str) {
  errno = 0;
  char *end;
  long long v = strtol(str, &end, 10);
  if (errno != 0 || end == str || *end != 0
      || v < 0
      || v > std::numeric_limits<unsigned int>::max()) {
    printf("Unable to parse this as an unsigned integer: %s", str);
    usage();
  }
  return v;
}

static void parse_args(int argc, const char **argv) {
  progname = argv[0];
  argc--; argv++;
  while (argc > 0) {
    std::string arg(*argv);
    argc--; argv++;
    if (arg == "--minscale") {
      min_scale = parse_uint(*argv);
      argc--; argv++;
    } else if (arg == "--maxscale") {
      max_scale = parse_uint(*argv);
      argc--; argv++;
    } else if (arg == "--iter") {
      n_iterations = parse_uint(*argv);
      argc--; argv++;
    } else {
      fprintf(stderr, "Don't understand this argument: %s\n", arg.c_str());
      usage();
    }
  }
}


int main(int argc, const char *argv[]) {
  parse_args(argc, argv);
  data_file = fopen("pagerank.data", "w");
  const int edges_per_vertex = 2;
  const int nfile = 1;
  for (int scale = min_scale; scale <= max_scale; scale++) {
    pagerankpipeline<uint32_t>(scale, edges_per_vertex, nfile, n_iterations);
  }
  fclose(data_file);
  data_file = NULL;
  return 0;
}

// Local Variables:
// mode: C++
// c-basic-offset: 2
// indent-tabs-mode: nil
// End:
