SHELL=/bin/bash
############################
# Info to specify
############################

CC      = clang
CXX     = clang++
MPICC   ?= OMPI_CC=clang mpicc
MPICXX  = OMPI_CXX=clang++ mpic++


TYPEART_RTPATH = ${TYPEART_PATH}/lib
LTYPEART_RT= -L$(TYPEART_RTPATH) -ltypeart-rt
PASSES=-load ${TYPEART_PATH}/lib/meminstfinderpass.so -load ${TYPEART_PATH}/lib/typeartpass.so
CALL_FILTER=-call-filter -call-filter-deep

SOURCES=api.c apigeom.c box.c global.c hash.c parallel.c threads.c camera.c render.c trace.c grid.c intersect.c sphere.c plane.c ring.c triangle.c cylinder.c quadric.c extvol.c vol.c ui.c util.c vector.c shade.c texture.c coordsys.c imap.c light.c imageio.c jpeg.c pngfile.c ppm.c sgirgb.c spectxt.c tgafile.c winbmp.c main.c getargs.c parse.c nffparse.c glwin.c spaceball.c trackball.c mgfparse.c ac3dparse.c

OBJS=$(SOURCES:.c=.o)

CFLAGS=-DSPEC_MPI -DNDEBUG -g
COPTIMIZE = -O2

LDFLAGS=-lm


.c.o:
ifeq ($(TYPEART), yes)
	$(MPICC) $(CFLAGS) -O1 -Xclang -disable-llvm-passes -S -emit-llvm $< -o - | opt $(PASSES) -typeart -typeart-stats -S | opt $(COPTIMIZE) -S | opt $(PASSES) -typeart -typeart-alloca -alloca-array-only=false $(CALL_FILTER) -typeart-no-heap=true -typeart-stats | llc -x=ir -filetype=obj -o $@
else
	$(MPICC) $(CFLAGS) $(COPTIMIZE) -c $< -o $@
endif


122.tachyon: $(OBJS)
ifeq ($(TYPEART), yes)
	$(MPICC) $(OBJS) $(LDFLAGS) $(LTYPEART_RT) -o $@
else
	$(MPICC) $(OBJS) $(LDFLAGS) -o $@
endif


clean:
	rm -f 122.tachyon $(OBJS) typeart-types.yaml
