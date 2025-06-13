#default build suggestion of MPI + OPENMP with gcc on Livermore machines you might have to change the compiler name

SHELL = /bin/sh
.SUFFIXES: .cc .o .ll

LULESH_EXEC = lulesh2.0

SERCXX = clang++ -DUSE_MPI=0
MPICXX ?= OMPI_CXX=clang++ mpic++
CXX = $(MPICXX) -DUSE_MPI=1

TYPEART_RTPATH = ${TYPEART_PATH}/lib
LTYPEART_RT= -L$(TYPEART_RTPATH) -ltypeart-rt 

SOURCES2.0 = \
	lulesh.cc \
	lulesh-comm.cc \
	lulesh-viz.cc \
	lulesh-util.cc \
	lulesh-init.cc
OBJECTS2.0 = $(SOURCES2.0:.cc=.o)

#Below are reasonable default flags for a serial build w/o OpenMP
CXXFLAGS = -g -I. -Wall
LDFLAGS = -g -O3

.cc.o: lulesh.h
	$(CXX) $(CXXFLAGS) -O3 -c -o $@ $<

all: $(LULESH_EXEC)

lulesh2.0: $(OBJECTS2.0)
	@echo "Linking" 
	$(CXX) $(OBJECTS2.0) $(LDFLAGS) -lm -o $@

clean:
	/bin/rm -f *.o *.s *~ $(OBJECTS) $(LULESH_EXEC) typeart-types.yaml
	/bin/rm -rf *.dSYM

tar: clean
	cd .. ; tar cvf lulesh-2.0.tar LULESH-2.0 ; mv lulesh-2.0.tar LULESH-2.0
