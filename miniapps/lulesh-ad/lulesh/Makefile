#default build suggestion of MPI + OPENMP with gcc on Livermore machines you might have to change the compiler name

SHELL = /bin/sh
.SUFFIXES: .cc .o .ll

LULESH_EXEC = lulesh2.0

SERCXX = clang++ -DUSE_MPI=0
MPICXX ?= OMPI_CXX=clang++ mpic++
CXX = $(MPICXX) -DUSE_MPI=1

MEDIPACK_DIR=../medi
CODIPACK_DIR=../codi

SOURCES2.0 = \
	lulesh.cc \
	lulesh-comm.cc \
	lulesh-viz.cc \
	lulesh-util.cc \
	lulesh-init.cc \
  lulesh-ad.cc
OBJECTS2.0 = $(SOURCES2.0:.cc=.o)

#Below are reasonable default flags for a serial build w/o OpenMP
CXXFLAGS = -g -I. -Wall -Wno-format-security -Wno-overloaded-virtual -I${MEDIPACK_DIR}/include -I${MEDIPACK_DIR}/src -I${CODIPACK_DIR}/include
LDFLAGS = -g -O3

.cc.o: lulesh.h
	$(CXX) $(CXXFLAGS) -O3 -c -o $@ $<

all: $(LULESH_EXEC)

lulesh2.0: $(OBJECTS2.0)
	@echo "Linking" 
	$(CXX) $(OBJECTS2.0) $(LDFLAGS) -lm -o $@

medi: medi-main.cpp
	$(CXX) $(CXXFLAGS) -O0 -o $@ $<

clean:
	/bin/rm -f *.o *.s *~ $(OBJECTS) $(LULESH_EXEC) typeart-types.yaml
	/bin/rm -rf *.dSYM

tar: clean
	cd .. ; tar cvf lulesh-2.0.tar LULESH-2.0 ; mv lulesh-2.0.tar LULESH-2.0
