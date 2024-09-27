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


  const int bcast_root = 1;
  int msg; int len = 1;

  if ( mpi_rank == bcast_root )
    msg = 1;

  // bcast stands for broadcast
  MPI_Bcast(&msg, len, MPI_INT, bcast_root, MPI_COMM_WORLD);

  if ( mpi_rank != bcast_root )
      printf( "MPI received at %d: %d\n", mpi_rank, msg );

  /*
   * MPI_Bcast is a only one example of many advanced group-communication
   * functions provided by MPI:
   * MPI_Reduce, MPI_Allreduce
   * MPI_Gather, MPI_Allgather, MPI_Gatherv, MPI_Allgatherv
   * MPI_Scatter, MPI_Scatterv
   * MPI_Reduce_scatter, MPI_Reduce_scatterv
   * MPI_Alltoall, MPI_Alltoallv
   */

  MPI_Finalize();


  return 0;
}
