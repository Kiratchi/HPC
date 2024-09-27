#define _XOPEN_SOURCE 700

#include <math.h>
#include <mpi.h>
#include <omp.h>
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
  int provided;
  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
  if ( provided != MPI_THREAD_FUNNELED )
    exit(1);

  // If you are in a parallel environment, you need to use the master construct
  // to invoke MPI_Init_thread.
  /*
   * #pragma omp master
   * {
   *   int provided;
   *   MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
   *   if ( provided != MPI_THREAD_FUNNELED )
   *     exit(1);
   * }
   */

  // Alternatively, you could use
  // MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  // but this is not provided by all common MPI libraries.

  int nmb_mpi_proc, mpi_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &nmb_mpi_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  if ( mpi_rank == 0 ) {
    printf( "Number of processes: %d\n", nmb_mpi_proc );

    {
      int msg = 1; int len = 1; int dest_rank = 1; int tag = 1;
      MPI_Send(&msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
      printf( "MPI message sent from %d: %d\n", mpi_rank, msg );
    }

    {
      int msg; int max_len = 1; int src_rank = 1; int tag = 1;
      MPI_Recv(&msg, max_len, MPI_INT, src_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf( "MPI message received at %d: %d\n", mpi_rank, msg );
    }
  }
  
  // NOTE: We will print in one of worker processes here. This is only OK,
  // because we know exactly the setup in which we run this example program and
  // can live with interleaved text output, if it happens occassionally.
  else if ( mpi_rank == 1 ) {
    int msg;
    
    {
      int max_len = 1; int src_rank = 0; int tag = 1;
      MPI_Recv(&msg, max_len, MPI_INT, src_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf( "MPI message received at %d: %d\n", mpi_rank, msg );
    }

    #pragma omp parallel for reduction(+:msg)
    for ( int i = 0; i < 8; ++i )
      ++msg;

    {
      int len = 1; int dest_rank = 0; int tag = 1;
      MPI_Send(&msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
      printf( "MPI message sent from %d: %d\n", mpi_rank, msg );
    }


  }

  MPI_Finalize();


  return 0;
}
