# Pagerank Pipeline Benchmark in C++
# Copyright 2015 Bradley C. Kuszmaul, bradley@mit.edu
OPTFLAGS = -O2
DIALECT = -std=c++11 -fcilkplus -fopenmp
CXXFLAGS = $(DIALECT) $(OPTFLAGS) -W -Wall -Werror -g
LDFLAGS  = $(DIALECT) $(OPTFLAGS)

default: testkernel3 runpagerankpipeline test-cache-friendly check

runpagerankpipeline: runpagerankpipeline.o krongraph500.o pagerankpipeline.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
runpagerankpipeline.o: runpagerankpipeline.cc pagerankpipeline.hh
krongraph500.o: krongraph500.cc krongraph500.hh
pagerankpipeline.o: pagerankpipeline.cc krongraph500.hh fasttime.h pagerankpipeline.hh csc.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

testkernel3: testkernel3.cc pagerankpipeline.o krongraph500.o fasttime.h krongraph500.hh csc.hh pagerankpipeline.hh
	$(CXX) $(CXXFLAGS) testkernel3.cc pagerankpipeline.o krongraph500.o -o $@

check: testkernel3 runpagerankpipeline test-cache-friendly
	./test-cache-friendly
	./testkernel3
	./runpagerankpipeline --minscale 10 --maxscale 10

test-cache-friendly.o cache-friendly.o: cache-friendly.hh
test-cache-friendly: test-cache-friendly.o cache-friendly.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm -rf runpagerankpipeline test-cache-friendly *.o
