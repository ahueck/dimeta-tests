MODEL ?= serial
MPICXX ?= OMPI_CC=clang mpicxx
MPICC ?= OMPI_CC=clang mpicc

export MPICC
export MPICXX
export ROCM_ARCH

.PHONY: all
all:
	chmod +x ./build.sh
	./build.sh $(MODEL)

.PHONY: clean
clean:
	chmod +x ./build.sh
	./build.sh $(MODEL) clean
