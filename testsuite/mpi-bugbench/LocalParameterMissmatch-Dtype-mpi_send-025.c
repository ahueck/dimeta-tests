// RUN: %typeart-mpicc -O2 -g %s -o %ditests_test_dir/%basename_t.exe
// RUN: %must-mpiexec -n 2 %ditests_test_dir/%basename_t.exe
// RUN: cat %must-output-json | %filecheck %s


// CHECK: MUST_ERROR_TYPEMATCH_MISMATCH

/* ///////////////////////// The MPI Bug Bench ////////////////////////

  Description: datatype missmatch: Buffer: MPI_UINT64_T MPI_Call: MPI_INT

  Version of MPI: 1.0

  Category: dtype

BEGIN_MBB_TESTS
  $ mpirun -np 2 ${EXE}
  | ERROR LocalParameterMissmatch
  | LocalParameterMissmatch-Dtype-mpi_send
END_MBB_TESTS
//////////////////////       End of MBI headers        /////////////////// */

#include <mpi.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int nprocs = -1;
  int rank = -1;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (nprocs < 2)
    printf(
        "MBB ERROR: This test needs at least 2 processes to produce a bug!\n");

  int *buf = (int *)calloc(10, sizeof(int));

  uint64_t *buf_mpi_uint64_t = (uint64_t *)calloc(10, sizeof(uint64_t));

  signed int *buf_mpi_int = (signed int *)calloc(10, sizeof(signed int));

  if (rank == 0) {
    /*MBBERROR_BEGIN*/ MPI_Recv(buf_mpi_uint64_t, 10, MPI_INT, 1, 0,
                                MPI_COMM_WORLD,
                                MPI_STATUS_IGNORE); /*MBBERROR_END*/
  }
  if (rank == 1) {
    /*MBBERROR_BEGIN*/ MPI_Send(buf_mpi_uint64_t, 10, MPI_INT, 0, 0,
                                MPI_COMM_WORLD); /*MBBERROR_END*/
  }
  free(buf);
  free(buf_mpi_uint64_t);
  free(buf_mpi_int);

  MPI_Finalize();
  printf("Rank %d finished normally\n", rank);
  return 0;
}
