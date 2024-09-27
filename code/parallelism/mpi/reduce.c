#define _XOPEN_SOURCE 700

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

int
main(
    int argc,
    char * argv[]
    )
{
  MPI_Init(&argc, &argv);

  int nmb_mpi_proc, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &nmb_mpi_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);


  const int red_root = 1;
  int as[] = {mpi_rank, mpi_rank+1, mpi_rank+2};
  int sum[3]; int len = 3;

  MPI_Reduce(as, sum, len, MPI_INT, MPI_SUM, red_root, MPI_COMM_WORLD);

  if ( mpi_rank == red_root )
      printf( "MPI received at %d: %d %d %d\n", mpi_rank, sum[0], sum[1], sum[2] );

  /* reduction operations in MPI are
   * MPI_MAX, MPI_MIN
   * MPI_MAXLOC, MPI_MINLOC (minimum in first component)
   * MPI_SUM, MPI_PROD
   * MPI_LAND, MPI_LOR, MPI_LXOR (logic &&, ||, ^)
   * MPI_BAND, MPI_BOR, MPI_BXOR (bitwise &&, ||, ^)
   */

  /* MPI_MAXLOC, MPI_MINLOC require a pair data structure. Predefined ones are
   * MPI_FLOAT_INT
   * MPI_DOUBLE_INT
   * MPI_LONG_DOUBLE_INT
   * MPI_SHORT_INT
   * MPI_2INT
   * MPI_LONG_INT
   */

  MPI_Finalize();


  return 0;
}
