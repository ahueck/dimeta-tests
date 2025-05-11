build_dir := build
MODEL := serial
MPICXX ?= OMPI_CC=clang mpicxx
MPICC ?= OMPI_CC=clang mpicc

.PHONY: all
all:
	TYPEART_WRAPPER=OFF cmake \
	    -DENABLE_MPI=ON -B"$(build_dir)" -DMODEL=$(MODEL) -DCMAKE_BUILD_TYPE=Release \
	    -DCMAKE_CXX_COMPILER=$(MPICXX) -DCMAKE_C_COMPILER=$(MPICC)
	cmake --build "$(build_dir)" --clean-first -- -j1
	cp "$(build_dir)"/serial-tealeaf .

.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(build_dir) serial-tealeaf
