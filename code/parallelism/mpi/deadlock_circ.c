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


  int len = 10000000;
  int *msg = (int*) calloc(len, sizeof(int));

  if ( mpi_rank == 0 ) {
    printf( "Number of processes: %d\n", nmb_mpi_proc );


    {
      int dest_rank = 1; int tag = 1;
      MPI_Send(msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
      printf( "MPI message sent from %d: %d\n", mpi_rank, msg[0] );
    }

    {
      int src_rank = 2; int tag = 1;
      MPI_Recv(msg, len, MPI_INT, src_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf( "MPI message received at %d: %d\n", mpi_rank, msg[0] );
    }
  }
  
  else if ( mpi_rank == 1 ) {
    {
      int dest_rank = 2; int tag = 1;
      MPI_Send(msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
      printf( "MPI message sent from %d: %d\n", mpi_rank, msg[0] );
    }

    {
      int src_rank = 0; int tag = 1;
      MPI_Recv(msg, len, MPI_INT, src_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf( "MPI message received at %d: %d\n", mpi_rank, msg[0] );
    }
  }

  else if ( mpi_rank == 2 ) {
    {
      int dest_rank = 0; int tag = 1;
      MPI_Send(msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
      printf( "MPI message sent from %d: %d\n", mpi_rank, msg[0] );
    }

    {
      int src_rank = 1; int tag = 1;
      MPI_Recv(msg, len, MPI_INT, src_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf( "MPI message received at %d: %d\n", mpi_rank, msg[0] );
    }
  }

  free(msg);

  MPI_Finalize();


  return 0;
}
