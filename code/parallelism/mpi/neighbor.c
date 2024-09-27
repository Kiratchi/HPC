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


  int status;
  MPI_Topo_test(MPI_COMM_WORLD, &status);
  if ( status == MPI_UNDEFINED ) {
    fprintf(stderr, "no topology defined at %d\n", mpi_rank);
    MPI_Finalize();
    return 0;
  }


  // this is zero initialized
  int isneighbor[nmb_mpi_proc];
  int one = 1;

  MPI_Neighbor_allgather(&one, 1, MPI_INT, isneighbor, 1, MPI_INT, MPI_COMM_WORLD);

  for ( int ix = 0; ix < nmb_mpi_proc; ++ix ) {
    if ( ix == mpi_rank )
      continue;
    if ( isneighbor[ix] )
      printf("neightbor of %d: %d\n", mpi_rank, ix);
  }


  MPI_Finalize();


  return 0;
}
