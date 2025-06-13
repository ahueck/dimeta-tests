SHELL=/bin/bash
############################
# Info to specify
############################
EXEBASE	 = spec_mpi.104.milc

CC	= clang
CXX     = clang++
MPICC	?= OMPI_CC=clang mpicc
MPICXX	= OMPI_CXX=clang++ mpic++


CFLAGS      	  = -I. -DFN -DFAST -DCONGRAD_TMP_VECTORS -DDSLASH_TMP_LINKS -g -std=c90
LDFLAGS 				= -lm

COPTIMIZE        = -O2
CXXOPTIMIZE      = -O2

TYPEART_RTPATH = ${TYPEART_PATH}/lib
LTYPEART_RT= -L$(TYPEART_RTPATH) -ltypeart-rt
PASSES=-load ${TYPEART_PATH}/lib/meminstfinderpass.so -load ${TYPEART_PATH}/lib/typeartpass.so
CALL_FILTER=-call-filter -call-filter-deep


# Sources
SOURCES= control.c f_meas.c gauge_info.c setup.c update.c update_h.c update_u.c layout_hyper_prime.c check_unitarity.c d_plaq4.c gaugefix2.c io_helpers.c io_lat4.c make_lattice.c path_product.c ploop3.c ranmom.c ranstuff.c reunitarize2.c gauge_stuff.c grsource_imp.c mat_invert.c quark_stuff.c rephase.c cmplx.c addmat.c addvec.c clear_mat.c clearvec.c m_amatvec.c m_mat_an.c m_mat_na.c m_mat_nn.c m_matvec.c make_ahmat.c rand_ahmat.c realtr.c s_m_a_mat.c s_m_a_vec.c s_m_s_mat.c s_m_vec.c s_m_mat.c su3_adjoint.c su3_dot.c su3_rdot.c su3_proj.c su3mat_copy.c submat.c subvec.c trace_su3.c uncmp_ahmat.c msq_su3vec.c sub4vecs.c m_amv_4dir.c m_amv_4vec.c m_mv_s_4dir.c m_su2_mat_vec_n.c l_su2_hit_n.c r_su2_hit_a.c m_su2_mat_vec_a.c gaussrand.c byterevn.c m_mat_hwvec.c m_amat_hwvec.c dslash_fn2.c d_congrad5_fn.c com_mpi.c io_nonansi.c


OBJS=$(SOURCES:.c=.o)

all: 104.milc

.c.o:
ifeq ($(TYPEART), yes)
	$(MPICC) $(CFLAGS) -O1 -Xclang -disable-llvm-passes -S -emit-llvm $< -o - | opt $(PASSES) -typeart -typeart-stats -S | opt $(COPTIMIZE) -S | opt $(PASSES) -typeart -typeart-alloca -alloca-array-only=false $(CALL_FILTER) -typeart-no-heap=true -typeart-stats | llc -x=ir -filetype=obj -o $@
else
	$(MPICC) -c $(CFLAGS) $(COPTIMIZE) $< -o $@
endif

104.milc: $(OBJS)
ifeq ($(TYPEART), yes)
	$(MPICC) $(OBJS) $(LDFLAGS) $(LTYPEART_RT) -o $@
else
	$(MPICC) $(OBJS) $(LDFLAGS) -o $@
endif


clean:
	rm -f $(OBJS) 104.milc typeart-types.yaml
