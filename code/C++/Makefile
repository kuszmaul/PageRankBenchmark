# Pagerank Pipeline Benchmark in C++
# Copyright 2015 Bradley C. Kuszmaul, bradley@mit.edu
OPTFLAGS = -O2
DIALECT = -std=c++11 
CXXFLAGS = $(DIALECT) $(OPTFLAGS) -W -Wall -Werror -g
LDFLAGS  = $(DIALECT) $(OPTFLAGS)

default: testkernel3 runpagerankpipeline runpagerankpipeline_omp

runpagerankpipeline: runpagerankpipeline.o krongraph500.o pagerankpipeline.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
runpagerankpipeline.o: runpagerankpipeline.cc pagerankpipeline.hh
krongraph500.o: krongraph500.cc krongraph500.hh
pagerankpipeline.o: pagerankpipeline.cc krongraph500.hh fasttime.h pagerankpipeline.hh csc.hh
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<
pagerankpipeline_omp.o: pagerankpipeline.cc krongraph500.hh fasttime.h pagerankpipeline.hh csc.hh
	$(CXX) $(CXXFLAGS) -DUSE_OMP -fopenmp $(LDFLAGS) -c -o $@ $<

runpagerankpipeline_omp: runpagerankpipeline.o krongraph500.o pagerankpipeline_omp.o
	$(CXX) $(CXXFLAGS) -fopenmp $(LDFLAGS) $^ $(LDLIBS) -o $@



testkernel3: testkernel3.cc pagerankpipeline.o krongraph500.o fasttime.h krongraph500.hh csc.hh pagerankpipeline.hh
	$(CXX) $(CXXFLAGS) testkernel3.cc pagerankpipeline.o krongraph500.o -o $@

check: testkernel3
	./testkernel3

clean:
	rm -rf runpagerankpipeline *.o
