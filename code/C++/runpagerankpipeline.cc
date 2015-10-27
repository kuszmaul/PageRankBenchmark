/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/* Pagerank Pipeline Benchmark in C++                          */
/* Copyright 2015 Bradley C. Kuszmaul, bradley@mit.edu         */

#include "pagerankpipeline.hh"

#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <limits>

static const int min_scale_default = 10;
static const int max_scale_default = 22;

static char *progname;

static void usage() __attribute__((noreturn));
static void usage() {
  printf("Usage: %s [ min_scale [ max_scale ]]\n", progname);
  printf("  Runs the page rank pipeline on a range of scales\n");
  printf("  low_scale defaults to %d\n", min_scale_default);
  printf("  hi_scale defaults to %d\n", max_scale_default);
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

int main(int argc, char *argv[] __attribute__((unused))) {
  progname = argv[0];
  const int min_scale = (argc >= 2) ? parse_uint(argv[1]) : min_scale_default;
  const int max_scale = (argc >= 3) ? parse_uint(argv[2]) : max_scale_default;
  if (argc >= 4) {
    printf("Too many arguments\n");
    usage();
  }
  data_file = fopen("c++.data", "a");
  assert(data_file);
  
  {
    char hostname[100];
    gethostname(hostname, sizeof(hostname)-1);
    hostname[sizeof(hostname)-1] = 0;
    const time_t now = time(NULL);
    struct tm *gmnow = gmtime(&now);
    char timestring[100];
    strftime(timestring, sizeof(timestring)-1, "%F %T", gmnow);
    timestring[sizeof(timestring)-1] = 0;
    fprintf(data_file, "# C++ run on %s by %s@%s\n", timestring, getlogin(), hostname);
    fprintf(data_file, "# scale k0-edges-per-sec k1-edges-per-sec k2-edges-per-sec k3-edges-per-sec\n");
  }
  const int edges_per_vertex = 16;
  const int nfile = 1;
  for (int scale = min_scale; scale <= max_scale; scale++) {
    pagerankpipeline<uint32_t>(scale, edges_per_vertex, nfile);
  }
  fclose(data_file);
  data_file = NULL;
  return 0;
}
