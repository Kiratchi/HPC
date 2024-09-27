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


  MPI_Comm sub_comm;

  if (mpi_rank < 4)
    MPI_Comm_split(MPI_COMM_WORLD, 0, 0, &sub_comm);
  else if (mpi_rank >= 4 && mpi_rank % 2 == 0)
    MPI_Comm_split(MPI_COMM_WORLD, 1, nmb_mpi_proc - mpi_rank, &sub_comm);
  else 
    MPI_Comm_split(MPI_COMM_WORLD, 2, 0, &sub_comm);


  MPI_Group sub_group;
  int sub_group_size, sub_group_rank;

  MPI_Comm_group(sub_comm, &sub_group);
  MPI_Group_size(sub_group, &sub_group_size);
  MPI_Comm_rank(sub_comm, &sub_group_rank);

  printf( "mpi_rank, sub_group_rank, sub_group_size: %d %d %d\n",
          mpi_rank, sub_group_rank, sub_group_size );


  MPI_Finalize();


  return 0;
}
