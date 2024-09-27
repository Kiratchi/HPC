#define _XOPEN_SOURCE 700

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>


typedef struct {
  int *c;
  mtx_t *mtx;
} thrd_info_t;

int
main_thrd(
    void *args
    )
{
  const thrd_info_t *thrd_info = (thrd_info_t*) args;
  int* c = thrd_info->c;

  mtx_lock(thrd_info->mtx);
  ++(*c);
  mtx_unlock(thrd_info->mtx);

  return 0;
}

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

    const int nthrds = 8;
    thrd_t thrds[nthrds];
    thrd_info_t thrds_info[nthrds];
    
    mtx_t mtx;
    mtx_init(&mtx, mtx_plain);

    for ( int tx = 0; tx < nthrds; ++tx ) {
      thrds_info[tx].c = &msg;
      thrds_info[tx].mtx = &mtx;
      int r = thrd_create(thrds+tx, main_thrd, (void*)(thrds_info+tx));
      if ( r != thrd_success ) {
        fprintf(stderr, "failed to create thread\n");
        exit(1);
      }
    }

    for ( int tx = 0; tx < nthrds; ++tx )
      thrd_join(thrds[tx], NULL);

    mtx_destroy(&mtx);

    {
      int len = 1; int dest_rank = 0; int tag = 1;
      MPI_Send(&msg, len, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
      printf( "MPI message sent from %d: %d\n", mpi_rank, msg );
    }


  }

  MPI_Finalize();


  return 0;
}
