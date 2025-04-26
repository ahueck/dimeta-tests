// RUN: %typeart-mpicc -O1 -g %s -o %ditests_test_dir/%basename_t.exe
// RUN: %must-mpiexec  -n 2 %ditests_test_dir/%basename_t.exe
// RUN: cat %must-output-json | %filecheck %s

// CHECK: MUST_ERROR_TYPEMATCH_MISMATCH

/* ///////////////////////// The MPI Bug Bench ////////////////////////

  Description: datatype missmatch: Buffer: MPI_INT MPI_Call:
MPI_UNSIGNED_LONG_LONG

  Version of MPI: 1.0

  Category: P2P

BEGIN_MBB_TESTS
  $ mpirun -np 2 ${EXE}
  | ERROR LocalParameterMissmatch
  | LocalParameterMissmatch-Dtype-mpi_isend
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
  MPI_Request mpi_request_0 = MPI_REQUEST_NULL;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (nprocs < 2)
    printf(
        "MBB ERROR: This test needs at least 2 processes to produce a bug!\n");

  int *buf = (int *)calloc(10, sizeof(int));

  signed int *buf_mpi_int = (signed int *)calloc(10, sizeof(signed int));

  unsigned long long int *buf_mpi_unsigned_long_long =
      (unsigned long long int *)calloc(10, sizeof(unsigned long long int));

  if (rank == 0) {
    /*MBBERROR_BEGIN*/ MPI_Irecv(buf_mpi_int, 10, MPI_UNSIGNED_LONG_LONG, 1, 0,
                                 MPI_COMM_WORLD,
                                 &mpi_request_0); /*MBBERROR_END*/
    MPI_Wait(&mpi_request_0, MPI_STATUS_IGNORE);
  }
  if (rank == 1) {
    /*MBBERROR_BEGIN*/ MPI_Isend(buf_mpi_int, 10, MPI_UNSIGNED_LONG_LONG, 0, 0,
                                 MPI_COMM_WORLD,
                                 &mpi_request_0); /*MBBERROR_END*/
    MPI_Wait(&mpi_request_0, MPI_STATUS_IGNORE);
  }
  free(buf);
  free(buf_mpi_int);
  free(buf_mpi_unsigned_long_long);

  MPI_Finalize();
  printf("Rank %d finished normally\n", rank);
  return 0;
}
