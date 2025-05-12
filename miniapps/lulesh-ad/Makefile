MPICXX ?= OMPI_CC=clang mpicxx
MPICC ?= OMPI_CC=clang mpicc

.PHONY: all
all:
	$(MAKE) -C lulesh all

.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	$(MAKE) -C lulesh clean
