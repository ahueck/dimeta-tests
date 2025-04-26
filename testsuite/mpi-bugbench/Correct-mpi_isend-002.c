// RUN: %typeart-mpicc -O1 -g %s -o %ditests_test_dir/%basename_t.exe
// RUN: %must-mpiexec  -n 2 %ditests_test_dir/%basename_t.exe
// RUN: cat %must-output-json | %filecheck %s

// CHECK-NOT: MUST_ERROR_TYPEMATCH_MISMATCH

/* ///////////////////////// The MPI Bug Bench ////////////////////////

  Description:

  Version of MPI: 1.0

  Category: P2P

BEGIN_MBB_TESTS
  $ mpirun -np 2 ${EXE}
  | OK
  | Correct-mpi_isend
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

  unsigned long int *buf_mpi_unsigned_long =
      (unsigned long int *)calloc(1, sizeof(unsigned long int));

  int *buf = (int *)calloc(10, sizeof(int));

  if (rank == 0) {
    MPI_Recv(buf_mpi_unsigned_long, 1, MPI_UNSIGNED_LONG, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }
  if (rank == 1) {
    MPI_Isend(buf_mpi_unsigned_long, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD,
              &mpi_request_0);
    MPI_Wait(&mpi_request_0, MPI_STATUS_IGNORE);
  }
  free(buf);

  MPI_Finalize();
  printf("Rank %d finished normally\n", rank);
  return 0;
}
