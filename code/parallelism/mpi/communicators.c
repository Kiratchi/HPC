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


  if ( nmb_mpi_proc < 7 ) {
    fprintf(stderr, "insufficient number (%d) of processed", nmb_mpi_proc);
    exit(1);
  }


  MPI_Group sub_group, world_group;
  MPI_Comm sub_comm_0, sub_comm_1;

  int sub_group_ranks[] = {0,1,6};
  
  MPI_Comm_group(MPI_COMM_WORLD, &world_group);
  MPI_Group_incl(world_group, 3, sub_group_ranks, &sub_group);
  
  MPI_Comm_create(MPI_COMM_WORLD, sub_group, &sub_comm_0);
  MPI_Comm_create(MPI_COMM_WORLD, sub_group, &sub_comm_1);


  // Only ranks 0, 1, and 6 may participate in the communication via sub_comm_0
  // and sub_comm_1.
  if ( mpi_rank == 0 || mpi_rank == 1 || mpi_rank == 6 ) {
    const int bcast_root = 1;
    int msg; int len = 1;

    if ( mpi_rank == bcast_root )
      msg = 1;

    // bcast stands for broadcast
    MPI_Bcast(&msg, len, MPI_INT, bcast_root, sub_comm_0);

    if ( mpi_rank != bcast_root )
        printf( "MPI received at %d: %d\n", mpi_rank, msg );
  }


  MPI_Finalize();


  return 0;
}
