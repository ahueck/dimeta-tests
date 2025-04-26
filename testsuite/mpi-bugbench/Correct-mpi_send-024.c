// RUN: %typeart-mpicc -O2 -g %s -o %ditests_test_dir/%basename_t.exe
// RUN: %must-mpiexec -n 2 %ditests_test_dir/%basename_t.exe
// RUN: cat %must-output-json | %filecheck %s


// CHECK-NOT: MUST_ERROR_TYPEMATCH_MISMATCH

/* ///////////////////////// The MPI Bug Bench ////////////////////////

  Description:

  Version of MPI: 1.0

  Category: dtype

BEGIN_MBB_TESTS
  $ mpirun -np 2 ${EXE}
  | OK
  | Correct-mpi_send
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

  uint32_t *buf_mpi_uint32_t = (uint32_t *)calloc(1, sizeof(uint32_t));

  int *buf = (int *)calloc(10, sizeof(int));

  if (rank == 0) {
    MPI_Recv(buf_mpi_uint32_t, 1, MPI_UINT32_T, 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
  }
  if (rank == 1) {
    MPI_Send(buf_mpi_uint32_t, 1, MPI_UINT32_T, 0, 0, MPI_COMM_WORLD);
  }
  free(buf);

  MPI_Finalize();
  printf("Rank %d finished normally\n", rank);
  return 0;
}
